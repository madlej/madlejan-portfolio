# steel-sheet-pricer

Multithreaded pricing engine for welded steel sheets based on customer orders and supplier offers.
Implements a producer-consumer model using C++11 threads and synchronization.

Customer requests are priced using prefabricated plates from multiple suppliers, considering welding cost and plate composition.
The project is based on a university assignment (C++ multithreading and design).

Uses a solver based on 2D cutting stock problem
Handles asynchronous suppliers and multiple parallel customer orders.

The testing framework is not included in this repository.
