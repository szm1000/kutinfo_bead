#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

template<typename T>
struct vec2{
    T x, y;

    vec2<T>& operator=(vec2<T> const& v){
        x = v.x;
        y = v.y;
        return *this;
    }

    template<typename T1>
    auto& operator+=(vec2<T1> const& v){
        x += v.x;
        y += v.y;
        return *this;
    }

    template<typename T1>
    auto& operator-=(vec2<T1> const& v){
        x -= v.x;
        y -= v.y;
        return *this;
    }

    template<typename T1>
    auto& operator*=(T1 const& c){
        x *= c;
        y *= c;
        return *this;
    }

    template<typename T1>
    auto& operator/=(T1 const& d){
        x /= d;
        y /= d;
        return *this;
    }
};

template<typename T1, typename T2>
auto operator+(vec2<T1> const& v1,vec2<T2> const& v2){
    return vec2<decltype(v1.x+v2.x)>{v1.x+v2.x,v1.y+v2.y};
}

template<typename T1, typename T2>
auto operator-(vec2<T1> const& v1,vec2<T2> const& v2){
    return vec2<decltype(v1.x-v2.x)>{v1.x-v2.x,v1.y-v2.y};
}

template<typename T1, typename T2>
auto operator*(T1 const& c,vec2<T2> const& v){
    return vec2<decltype(c * v.x)>{c * v.x, c * v.y};
}

template<typename T1, typename T2>
auto operator*(vec2<T1> const& v, T2 const& c){
    return vec2<decltype(v.x * c)>{v.x * c, v.y * c};
}

template<typename T1, typename T2>
auto operator/(vec2<T1> const& v, T2 const& d){
    return vec2<decltype(v.x / d)>{v.x / d , v.y / d};
}

template<typename T>
std::ostream& operator<<(std::ostream& o,vec2<T> const& v){
    o<<v.x<<','<<v.y;
    return o;
}

template<typename T>
std::istream& operator>>(std::istream& s,vec2<T>& v){
    auto rewind = [state = s.rdstate(), pos = s.tellg(), &s](){s.seekg(pos); s.setstate(state);};   
    std::string temp;
    std::getline(s, temp);
    if(!s){rewind(); std::cout<<"Read error!"<<std::endl;}
    std::stringstream ss(temp);
    if(!ss){rewind(); std::cout<<"Read error!"<<std::endl;}
    std::getline(ss, temp, ',');
    if(static_cast<int>(temp.size()) <= 0){
        rewind();
        return s;
    }
    std::stringstream vx_ss(temp);
    vx_ss>>v.x;
    std::getline(ss, temp);
    if(static_cast<int>(temp.size()) <= 0){
        rewind();
        return s;
    }
    std::stringstream vy_ss(temp);
    vy_ss>>v.y;

    return s;
}

template<typename T1,typename T2>
auto dot(vec2<T1> const& v1, vec2<T2> const& v2){
    return v1.x * v2.x + v1.y * v2.y;
}

template<typename T>
auto length(vec2<T> const& v){
    return std::sqrt(dot(v, v));
}

template<typename T>
auto sqlength(vec2<T> const& v){
    return dot(v, v);
}

template<typename T>
vec2<T> normalize(vec2<T> const& v){
    T l = length(v);
    if(l < 1e-12){
        std::cout<<"Vector is a null vector!"<<std::endl;
        return v;
    }
    return v / l;
}



template<typename T>
bool vec2eq(vec2<T> const& v1, vec2<T> const& v2, T eps){
    if(std::abs(v1.x - v2.x) < eps && std::abs(v1.y -v2.y) < eps){
        return true;
    }
    return false;
}
