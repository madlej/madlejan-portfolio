package cz.cvut.fit.tjv.omdsserver.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;
import org.hibernate.proxy.HibernateProxy;

import java.util.*;

@Getter
@Setter
@Entity
@Table(name = "planet")
public class Planet implements EntityWithId<String> {

    @Id
//    @GeneratedValue(strategy = GenerationType.SEQUENCE)             //... pote smazat, nyni pro automaticke generovani id (pote bude na zaklade jmen planet dano pevne)
    @Column(name = "name_planet", nullable = false, unique = true)
    private String namePlanet;

    @Column(name = "type_planet")
    private String typePlanet;

    @Column(name = "mass")
    private Integer mass;


    @OneToMany(mappedBy = "planet", cascade = CascadeType.REMOVE, orphanRemoval = true)
    private List<Moon> moons = new LinkedList<>();

    @Override
    public final boolean equals(Object o) {
        if (this == o) return true;
        if (o == null) return false;
        Class<?> oEffectiveClass = o instanceof HibernateProxy ? ((HibernateProxy) o).getHibernateLazyInitializer().getPersistentClass() : o.getClass();
        Class<?> thisEffectiveClass = this instanceof HibernateProxy ? ((HibernateProxy) this).getHibernateLazyInitializer().getPersistentClass() : this.getClass();
        if (thisEffectiveClass != oEffectiveClass) return false;
        Planet planet = (Planet) o;
        return getNamePlanet() != null && Objects.equals(getNamePlanet(), planet.getNamePlanet());
    }

    @Override
    public final int hashCode() {
        return this instanceof HibernateProxy ? ((HibernateProxy) this).getHibernateLazyInitializer().getPersistentClass().hashCode() : getClass().hashCode();
    }


    @Override
    public String getId() { return namePlanet; }
}