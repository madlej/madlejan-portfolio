package cz.cvut.fit.tjv.omdsserver.service;

import cz.cvut.fit.tjv.omdsserver.domain.EntityWithId;
import jakarta.persistence.EntityNotFoundException;
import org.springframework.data.annotation.Id;
import org.springframework.data.repository.CrudRepository;

import java.util.Optional;

public abstract class CrudServiceImpl<T extends EntityWithId<ID>, ID> implements CrudService<T, ID> {

//    protected CrudRepository<T, ID> repository;
//    public CrudServiceImpl(CrudRepository<T, ID> repository) {
//        this.repository = repository;
//    }
    protected abstract CrudRepository<T, ID> getCrudRepository();

    @Override
    public T create(T entity) {
        if (getCrudRepository().existsById(entity.getId()) || entity.getId() == null)
            throw new IllegalArgumentException();
        return getCrudRepository().save(entity);
    }

    @Override
    public Optional<T> findById(ID id) {
        return getCrudRepository().findById(id);
    }

    @Override
    public Iterable<T> findAll() {
        return getCrudRepository().findAll();
    }

    @Override
    public T update(ID id, T entity) {
        if(!getCrudRepository().existsById(entity.getId()) || entity.getId() == null)
            throw new IllegalArgumentException();
        return getCrudRepository().save(entity);
    }

    @Override
    public void deleteById(ID id) {
        if(!getCrudRepository().existsById(id))
            throw new IllegalArgumentException();
        getCrudRepository().deleteById(id);
    }
}


