#include <iostream>
#include <vector>./ropesim -s 32

#include "CGL/vector2D.h"

#include "mass.h"
#include "rope.h"
#include "spring.h"

namespace CGL {

    // start -> 起始节点的坐标，end -> 最终节点的坐标，num_nodes -> 节点个数
    // node_mass -> 节点的质量，k -> 弹簧系数
    // pinned_nodes -> 弹簧两个端点的索引
    Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
    {
        // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.
        //        Comment-in this part when you implement the constructor
        //        for (auto &i : pinned_nodes) {
        //            masses[i]->pinned = true;
        //        }
        for (int i = 0; i < num_nodes; ++i)
        {
            // 转成double再做除法，否则就会截断成int
            Vector2D pos = start + (end - start) * ((double)i / ((double)num_nodes - 1.0));
            masses.push_back(new Mass(pos, node_mass, false)); // 放质点
        }
        for (int i = 0; i < num_nodes - 1; ++i)
            springs.push_back(new Spring(masses[i], masses[i + 1], k)); // 放绳子
        for (auto &i : pinned_nodes)
            masses[i]->pinned = true;
    }

    void Rope::simulateEuler(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 2): Use Hooke's law to calculate the force on a node
            Vector2D ab = s->m2->position - s->m1->position;
            Vector2D f =  s->k *  (ab.unit()) * (ab.norm() - s->rest_length);
            s->m1->forces += f;
            s->m2->forces -= f;
        }

        for (auto &m : masses)
        {
            float k_d  = 0.1;
            if (!m->pinned)
            {
                // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
                m->forces += gravity * m->mass;
                // TODO (Part 2): Add global damping
                m->forces += - k_d * m->velocity;
                Vector2D a = m->forces / m->mass;
                /*
                     //explicit  Euler
                 m->position += m->velocity * delta_t;
                 m->velocity += a * delta_t;
                */
                //implicit Euler
                m->velocity += a * delta_t;
                m->position += m->velocity * delta_t;
            }
            // Reset all forces on each mass
            m->forces = Vector2D(0, 0);

        }
    }
    void Rope::simulateVerlet(float delta_t, Vector2D gravity)
    {
        for (auto &s : springs)
        {
            // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
            Vector2D ab = s->m2->position - s->m1->position;
            Vector2D f = s->k *  (ab.unit()) * (ab.norm() - s->rest_length);
            s->m1->forces += f;
            s->m2->forces -= f;
        }

        for (auto &m : masses)
        {
            if (!m->pinned)
            {
                m->forces += gravity * m->mass;
                Vector2D a = m->forces / m->mass;

                // TODO (Part 3.1): Set the new position of the rope mass
                Vector2D temp = m->position;
                // TODO (Part 4): Add global Verlet damping
                double  damping_factor = 0.00005;                // 核心公式，带阻尼的计算
                m->position = m->position +  (1 - damping_factor) * (m->position - m->last_position) + a * delta_t *delta_t;                // 用last_position记忆原先的位置
                m->last_position = temp;
            }
            m->forces =  Vector2D(0,0);
        }
    }
}
