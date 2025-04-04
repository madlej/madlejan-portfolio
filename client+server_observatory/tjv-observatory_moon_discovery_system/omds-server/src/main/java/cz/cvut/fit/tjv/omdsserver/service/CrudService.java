package cz.cvut.fit.tjv.omdsserver.service;

import java.util.Optional;

public interface CrudService<T, ID> {
    T create(T entity);
    Optional<T> findById(ID id);
    Iterable<T> findAll();
    T update(ID id, T entity);
    void deleteById(ID id);
}

