package cz.cvut.fit.tjv.omdsserver.controller.converter;

public interface converterDto <Dto, Entity> {
    Dto entityToDto(Entity entity);
    Entity dtoToEntity(Dto dto);
}
