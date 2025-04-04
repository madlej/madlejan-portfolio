package cz.cvut.fit.tjv.omdsserver.domain;

import com.fasterxml.jackson.annotation.JsonIgnore;
import jakarta.persistence.*;
import lombok.Getter;
import lombok.Setter;
import org.hibernate.proxy.HibernateProxy;

import java.util.*;

@Getter
@Setter
@Entity
@Table(name = "astronomer")
public class Astronomer implements EntityWithId<Long>{

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    @Column(name = "astronomer_id", nullable = false)
    private Long astronomerId;

    @Column(name = "real_name")
    private String realName;

    @Column(name = "email")
    private String email;


    @ManyToMany(mappedBy = "astronomers", cascade = {CascadeType.MERGE, CascadeType.PERSIST, CascadeType.DETACH})
//    @JsonIgnore
    private List<Moon> moons = new LinkedList<>();

    @Override
    public final boolean equals(Object o) {
        if (this == o) return true;
        if (o == null) return false;
        Class<?> oEffectiveClass = o instanceof HibernateProxy ? ((HibernateProxy) o).getHibernateLazyInitializer().getPersistentClass() : o.getClass();
        Class<?> thisEffectiveClass = this instanceof HibernateProxy ? ((HibernateProxy) this).getHibernateLazyInitializer().getPersistentClass() : this.getClass();
        if (thisEffectiveClass != oEffectiveClass) return false;
        Astronomer that = (Astronomer) o;
        return getAstronomerId() != null && Objects.equals(getAstronomerId(), that.getAstronomerId());
    }

    @Override
    public final int hashCode() {
        return this instanceof HibernateProxy ? ((HibernateProxy) this).getHibernateLazyInitializer().getPersistentClass().hashCode() : getClass().hashCode();
    }

    @Override
    public Long getId() { return this.astronomerId; }

}