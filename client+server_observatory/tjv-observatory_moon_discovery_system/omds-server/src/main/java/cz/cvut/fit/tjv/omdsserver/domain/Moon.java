package cz.cvut.fit.tjv.omdsserver.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;
import org.hibernate.proxy.HibernateProxy;

import java.time.LocalDate;
import java.util.*;

@Getter
@Setter
@Entity
@Table(name = "moon")
public class Moon implements EntityWithId<Long>{

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    @Column(name = "moon_id", nullable = false)
    private Long moonId;

    @Column(name = "discovery_date")                //... mozna pridat take nullable = true jelikoz je to povinny parametr???
    private LocalDate discoveryDate;

    @Column(name = "name_moon")
    private String nameMoon;

    @Column(name = "diameter")
    private Integer diameter;


    @ManyToMany(cascade = {CascadeType.MERGE, CascadeType.PERSIST, CascadeType.DETACH})
    @JoinTable(name = "moon_astronomers",
            joinColumns = @JoinColumn(name = "moon_moon_id"),
            inverseJoinColumns = @JoinColumn(name = "astronomers_astronomer_id"))
    private List<Astronomer> astronomers = new LinkedList<>();

    @ManyToOne
    @JoinColumn(name = "planet_name_planet")
    private Planet planet;


    @Override
    public final boolean equals(Object o) {
        if (this == o) return true;
        if (o == null) return false;
        Class<?> oEffectiveClass = o instanceof HibernateProxy ? ((HibernateProxy) o).getHibernateLazyInitializer().getPersistentClass() : o.getClass();
        Class<?> thisEffectiveClass = this instanceof HibernateProxy ? ((HibernateProxy) this).getHibernateLazyInitializer().getPersistentClass() : this.getClass();
        if (thisEffectiveClass != oEffectiveClass) return false;
        Moon moon = (Moon) o;
        return getMoonId() != null && Objects.equals(getMoonId(), moon.getMoonId());
    }

    @Override
    public final int hashCode() {
        return this instanceof HibernateProxy ? ((HibernateProxy) this).getHibernateLazyInitializer().getPersistentClass().hashCode() : getClass().hashCode();
    }

    @Override
    public Long getId() { return moonId; }
}