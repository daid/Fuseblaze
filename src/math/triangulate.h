#ifndef TRIANGULATE_H
#define TRIANGULATE_H

#include <sp2/math/vector.h>
#include <vector>

template<typename T> class Triangulate
{
public:
    typedef std::vector<sp::Vector2<T>> Path;
    
    static bool process(const Path& input, Path& output)
    {
        int n = input.size();
        if (n < 3)
            return false;

        int* indexes = new int[n];

        if (area(input) > 0.0f)
            for(int v=0; v<n; v++)
                indexes[v] = v;
        else
            for(int v=0; v<n; v++)
                indexes[v] = (n-1)-v;

        int nv = n;
        /*  remove nv-2 Vertices, creating 1 triangle every time */
        int count = 2*nv;   /* error detection */
        for(int m=0, v=nv-1; nv>2; )
        {
            /* if we loop, it is probably a non-simple polygon */
            if (count-- < 0)
            {
                //** Triangulate: ERROR - probable bad polygon!
                delete indexes;
                return false;
            }

            /* three consecutive vertices in current polygon, <u,v,w> */
            int u = v  ; if (nv <= u) u = 0;     /* previous */
            v = u+1; if (nv <= v) v = 0;     /* new v    */
            int w = v+1; if (nv <= w) w = 0;     /* next     */

            if (snip(input, u, v, w, nv, indexes))
            {
                int a, b, c, s, t;

                /* true names of the vertices */
                a = indexes[u]; b = indexes[v]; c = indexes[w];

                /* output Triangle */
                output.push_back(input[a]);
                output.push_back(input[b]);
                output.push_back(input[c]);

                m++;

                /* remove v from remaining polygon */
                for(s=v,t=v+1;t<nv;s++,t++)
                    indexes[s] = indexes[t];
                nv--;
                /* resest error detection counter */
                count = 2*nv;
            }
        }

        delete indexes;
        return true;
    }

private:
    static T area(const Path &input)
    {
        T result = 0;
        int p0 = input.size() - 1;
        for(unsigned int p1=0; p1<input.size(); p1++)
        {
            result += input[p0].x * input[p1].y - input[p1].x * input[p0].y;
            p0 = p1;
        }
        return result * 0.5f;
    }

    static bool inside(sp::Vector2<T> a, sp::Vector2<T> b, sp::Vector2<T> c, sp::Vector2<T> p)
    {
        sp::Vector2<T> cb = c - b;
        sp::Vector2<T> ac = a - c;
        sp::Vector2<T> ba = b - a;
        
        sp::Vector2<T> pa = p - a;
        sp::Vector2<T> pb = p - b;
        sp::Vector2<T> pc = p - c;

        T cross_a = ba.cross(pa);
        T cross_b = cb.cross(pb);
        T cross_c = ac.cross(pc);
        return cross_a >= 0.0f && cross_b >= 0.0f && cross_c >= 0.0f;
    }
    
    static bool snip(const Path &input, int u, int v, int w, int n, int* indexes)
    {
        sp::Vector2<T> a = input[indexes[u]];
        sp::Vector2<T> b = input[indexes[v]];
        sp::Vector2<T> c = input[indexes[w]];

        if (0.0000000001f > (((b.x-a.x)*(c.y-a.y)) - ((b.y-a.y)*(c.x-a.x))) )
            return false;

        for(int index=0; index<n; index++)
        {
            if( (index == u) || (index == v) || (index == w) )
                continue;
            sp::Vector2<T> p = input[indexes[index]];
            if (inside(a, b, c, p))
                return false;
        }

        return true;
    }
};

#endif//TRIANGULATE_H
