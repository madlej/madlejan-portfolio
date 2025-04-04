#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <semaphore>
#include <atomic>
#include <condition_variable>
#include "sample_tester.h"

#define printMode false

class CWeldingCompany{
private:
    struct pair_hash {
        template <class T1, class T2>
        size_t operator()(const std::pair<T1, T2> &p) const {
            return std::hash<T1>{}(p.first) ^ std::hash<T2>{}(p.second);
        }
    };

    std::atomic<int> running_customers;

    std::vector<std::thread> worker_thr;
    std::vector<std::thread> customer_thr;

    std::vector<ACustomer> customers_vect;
    std::vector<AProducer> producers_vect;
    
    std::mutex mtx;
    std::mutex mtx_producers;
    std::mutex mtx_send;
    std::condition_variable cv;
    std::condition_variable cv_producers;

    struct customerOrderList {
        size_t customerId = 0;
        AOrderList orderList = nullptr;
    };
    
    struct orderListWaitingPrice {
        std::vector<APriceList> received_priceLists_vect;
        std::unordered_set<AProducer> from_producers_set;
        APriceList merged_priceList = nullptr;
    };
    
    struct readyToSolve {
        COrder order = COrder(0, 0, 0);
        size_t orderId = 0;
        AOrderList orderList = nullptr;
        size_t customerId = 0;
        APriceList priceList = nullptr;
    };
    

    std::queue<customerOrderList> received_orderLists_que;
    std::map<unsigned, orderListWaitingPrice> orderLists_waiting_price_map;
    std::map<unsigned, APriceList> stored_priceLists_map;
    std::queue<readyToSolve> ready_to_solve_que;
    std::vector<std::map<AOrderList, size_t>> completed_orders_vect;

public:
    static bool usingProgtestSolver(){
        return false;
    }
    static void seqSolve(APriceList priceList, COrder &order){
        #if printMode
        printf("Solving order: %d %d %f %f\n", order.m_W, order.m_H, order.m_WeldingStrength, order.m_Cost);
        #endif
    
        double x_width = order.m_W;
        double y_heigh = order.m_H;
        double welding = order.m_WeldingStrength;
        std::vector<std::vector<double>> price(x_width + 1, std::vector<double>(y_heigh + 1, DBL_MAX));
    
        //... Setup prices from priceList
        for(const CProd &c : priceList->m_List){
            if(c.m_W <= x_width && c.m_H <= y_heigh)
                price[c.m_W][c.m_H] = c.m_Cost;
                
            if(c.m_W <= y_heigh && c.m_H <= x_width)
                price[c.m_H][c.m_W] = c.m_Cost;
        }
    
        //... Iterate throw all posible combinations
        for(size_t x = 1; x <= x_width; x++){
            for(size_t y = 1; y <= y_heigh; y++){
    
                //... Vertical welding
                for(size_t i = 1; i < x; i++){
                    if(price[i][y] == DBL_MAX || price[x - i][y] == DBL_MAX)
                        continue;
                    
                    price[x][y] = std::min(price[x][y], price[i][y] + price[x - i][y] + welding * y);
                }
    
                //... Horizontal welding
                for(size_t j = 1; j < y; j++){
                    if(price[x][j] == DBL_MAX || price[x][y - j] == DBL_MAX)
                        continue;
                        
                    price[x][y] = std::min(price[x][y], price[x][j] + price[x][y - j] + welding * x);
                }
            }
        }
    
        order.m_Cost = price[x_width][y_heigh];
        
        #if printMode
        printf("Solver solved problem, final price: %f\n", order.m_Cost);
        #endif
    }
    void addProducer(AProducer prod){
        producers_vect.push_back(prod);
    }
    void addCustomer(ACustomer cust){
        customers_vect.push_back(cust);
    }
    void addPriceList(AProducer prod, APriceList priceList){
        std::unique_lock<std::mutex> locker(mtx_producers);
        orderListWaitingPrice &curr = orderLists_waiting_price_map[priceList->m_MaterialID];

        if(curr.merged_priceList != nullptr || exists_in_set(curr.from_producers_set, prod)){
            cv_producers.notify_all();
            return;
        }

        #if printMode
        printf("PriceList: %d was added to order_list_waiting_price_map\n", priceList);
        #endif

        curr.received_priceLists_vect.push_back(priceList);
        curr.from_producers_set.insert(prod);

        if(curr.received_priceLists_vect.size() >= producers_vect.size()){
            #if printMode
            printf("PriceLists with materialId: %d, are being sended to merger\n", priceList->m_MaterialID);
            #endif
            curr.merged_priceList = merge_priceList(curr.received_priceLists_vect);
        }

        cv_producers.notify_all();
    }

    bool exists_in_set(const std::unordered_set<AProducer> &set, AProducer producer){
        return set.find(producer) != set.end(); 
    }


    APriceList merge_priceList(const std::vector<APriceList> &priceList_vect){
        std::unordered_map<std::pair<unsigned, unsigned>, double, pair_hash> merger_map;
        
        for(const APriceList &p : priceList_vect){
            for(const CProd &c :  p->m_List){
                std::pair<unsigned, unsigned> dim = {std::min(c.m_W, c.m_H), std::max(c.m_W, c.m_H)};
                
                if (merger_map.find(dim) == merger_map.end() || merger_map[dim] > c.m_Cost) {
                    merger_map[dim] = c.m_Cost;
                }
            }
        }
        
        APriceList merged_priceList = std::make_shared<CPriceList>(priceList_vect[0]->m_MaterialID);
        for(const auto &it : merger_map){
            merged_priceList->add(CProd(it.first.first, it.first.second, it.second));
        }

        #if printMode
        printf("Merged priceList with id: %d was returned\n", merged_priceList->m_MaterialID);
        #endif

        return merged_priceList;
    }
    

    void start(unsigned thrCount){

        running_customers.store(customers_vect.size());
        completed_orders_vect.resize(customers_vect.size());  

        for(size_t i = 0; i < thrCount; i++)
            worker_thr.push_back( std::thread(&CWeldingCompany::worker_fnc, this) );

        for(size_t i = 0; i < customers_vect.size(); i++)
            customer_thr.push_back( std::thread(&CWeldingCompany::customer_fnc, this, customers_vect[i], i));
        
        
        #if printMode
        printf("All threads are running\n");
        #endif
        
    }
    void stop(){

        for(std::thread &t : customer_thr)
            t.join();

        for(std::thread &t : worker_thr)
            t.join();
        
    }

    void worker_fnc(){
        #if printMode
        printf("Worker thr was started\n");
        #endif

        while(true){
            std::unique_lock<std::mutex> locker(mtx);
            cv.wait(locker, [this] {return !received_orderLists_que.empty() || !ready_to_solve_que.empty() || running_customers.load() <= 0;} );
            
            if(received_orderLists_que.empty() && ready_to_solve_que.empty() && running_customers.load() <= 0){
                #if printMode
                printf("Worker thread is turning off\n");
                #endif
                break;
            }

            if(!received_orderLists_que.empty())
                manage_priceLists_and_containers_logic(locker);

            readyToSolve order_to_be_solved = ready_to_solve_que.front();
            ready_to_solve_que.pop();
            
            cv.notify_all();
            locker.unlock(); 

            seqSolve(order_to_be_solved.priceList, order_to_be_solved.order);

            std::unique_lock<std::mutex> locker_send(mtx_send);
            send_order(order_to_be_solved);
            cv.notify_all();            
        }
    }

    void send_order(readyToSolve solved_order){

        std::map<AOrderList, size_t> &customer_send_map = completed_orders_vect[solved_order.customerId];
        AOrderList &orderList_id = solved_order.orderList;
        
        //... Create new orderList if wasnt created
        auto it = customer_send_map.find(orderList_id);
        if(it == customer_send_map.end()){
            customer_send_map[orderList_id] = 0;
        }

        solved_order.orderList->m_List[solved_order.orderId].m_Cost = solved_order.order.m_Cost;
        customer_send_map[orderList_id]++;

        if(customer_send_map[orderList_id] == orderList_id->m_List.size()){
            customers_vect[solved_order.customerId]->completed(solved_order.orderList);

            #if printMode
            printf("Completed OrderList is beeing sended: ");
            for(const COrder &c : solved_order.orderList->m_List){
                printf("%u %u %f,  ", c.m_W, c.m_H, c.m_Cost);
            }
            printf("\n");
            #endif
        }
    }


    void manage_priceLists_and_containers_logic(std::unique_lock<std::mutex> &locker){
        customerOrderList cust_orderList = received_orderLists_que.front();
        received_orderLists_que.pop();
        
        APriceList priceList = get_priceList_if_already_stored(cust_orderList);
        
        if(priceList == nullptr){
            #if printMode
            printf("PriceList for materialId: %d from customer: %ld WANS'T found\n", cust_orderList.orderList->m_MaterialID, cust_orderList.customerId);
            #endif
            cv.notify_all();
            locker.unlock();
            priceList = get_priceList_from_producers(cust_orderList.orderList->m_MaterialID);               
            locker.lock();
            cv.wait(locker, [this, priceList] { return priceList != nullptr; });            
            if(get_priceList_if_already_stored(cust_orderList) == nullptr)
                stored_priceLists_map[cust_orderList.orderList->m_MaterialID] = priceList; 
        }else{
            #if printMode
            printf("PriceList for materialId: %d from customer: %ld was found in stored pricelists\n", cust_orderList.orderList->m_MaterialID, cust_orderList.customerId);
            #endif
        }
        
        //... Save orders to que for solver
        #if printMode
        printf("\n");
        #endif
        size_t index = 0;
        for(const COrder &order : cust_orderList.orderList->m_List){
            ready_to_solve_que.emplace(readyToSolve{order, index, cust_orderList.orderList, cust_orderList.customerId, priceList});
            index++;
            #if printMode
            printf("Order added to ready_to_solve_que: CusId: %zu  OrderList: %p  Height x Width: %u x %u\n", ready_to_solve_que.back().customerId, (void*)ready_to_solve_que.back().orderList.get(), ready_to_solve_que.back().order.m_H, ready_to_solve_que.back().order.m_W);
            #endif
        }
    }

    
    APriceList get_priceList_if_already_stored(const customerOrderList &c) const{
        auto it = stored_priceLists_map.find(c.orderList->m_MaterialID);
        if(it != stored_priceLists_map.end()){
            return it->second;
        }
        return nullptr;
    }

    APriceList get_priceList_from_producers(unsigned materialId){
        for(const AProducer &producer : producers_vect)
            producer->sendPriceList(materialId);
        
        std::unique_lock<std::mutex> locker(mtx_producers);
        cv_producers.wait(locker, [this, materialId] {
            auto it = orderLists_waiting_price_map.find(materialId);
            return it != orderLists_waiting_price_map.end() && it->second.merged_priceList != nullptr;
        });
        APriceList priceList = orderLists_waiting_price_map[materialId].merged_priceList;
        cv_producers.notify_all();
        return priceList;
    }


    


    void customer_fnc(const ACustomer& customer, size_t cust_id){
        #if printMode
        printf("Customer thr was started\n");
        #endif
        
        AOrderList orderList;
        while(true){
            orderList = customer->waitForDemand();

            if(orderList == nullptr){
                running_customers.fetch_add(-1);
                cv.notify_all();
                break;  
            }
            
            std::unique_lock<std::mutex> locker(mtx);
            received_orderLists_que.push({cust_id, orderList});
            cv.notify_all();
        }
    }

};

//-------------------------------------------------------------------------------------------------
int main(){
    using namespace std::placeholders;
    CWeldingCompany test;

    //... Inicialization of producents
    AProducer p1 = std::make_shared<CProducerSync>(std::bind(&CWeldingCompany::addPriceList, &test, _1, _2));
    AProducerAsync p2 = std::make_shared<CProducerAsync>(std::bind(&CWeldingCompany::addPriceList, &test, _1, _2));
    test.addProducer(p1);
    test.addProducer(p2);
    //... Inicialization of customers
    test.addCustomer(std::make_shared<CCustomerTest>(3));
    test.addCustomer(std::make_shared<CCustomerTest>(1));
    test.addCustomer(std::make_shared<CCustomerTest>(2));
    //... Running threads for async producers (sync producer doesn't have threads)
    p2->start();
    //... Run of working threads
    test.start(3);

    // std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    #if printMode
    printf("-------\n");
    #endif

    //... Stop of all threads
    test.stop();
    p2->stop();


    return EXIT_SUCCESS;
}
