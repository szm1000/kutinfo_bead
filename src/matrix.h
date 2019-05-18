#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <initializer_list>
#include <ostream>
#include <string>
#include <sstream>

namespace detail{
	template<typename M1, typename M2, typename F>
	void transform_matrix1(M1 const& m1, M2& m2, F f){
		std::transform(m1.cbegin(), m1.cend(), m2.begin(), f);
	}

	template<typename M1, typename M2, typename M3, typename F>
	void transform_matrix2(M1 const& m1, M2 const& m2, M3 & m3, F f){
		std::transform(m1.cbegin(), m1.cend(), m2.cbegin(), m3.begin(), f);
	}
}

auto add = [](auto const& x, auto const& y){ return x + y; };
auto sub = [](auto const& x, auto const& y){ return x - y; };

template<typename T>
std::vector<T> sq_mat_mul(std::vector<T> const& m1, std::vector<T> const& m2, int dim){
    int const size = static_cast<int>(m1.size());
    int n = dim*dim;
    T sum = 0;
    std::vector<T> temp(size);
    if(size !=  static_cast<int>(m2.size()) || n != size){
        std::cout<<"Matrices cannot be multiplied together!"<<std::endl;
        return temp;
    }
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            sum = 0;
            for(int k = 0; k < dim; k++){
                sum += (m1[i * dim + k]) * (m2[k * dim + j ]);
            }
        temp[dim*i+j] = sum;
        }
    }
    return temp;
}

template<typename T>
class sq_matrix{

    int dim;
    std::vector<T> data;

    public:

    sq_matrix(): dim{1}, data{{0.0}}{};
    sq_matrix(int i): dim(i), data(i*i,0){};
    sq_matrix(int i, std::vector<T> const& v):dim(i) {
        if(dim * dim == static_cast<int>(v.size())){
            data = v;
        }
        else{
            std::cout<<"Initialization error!"<<std::endl;
            exit(-1);
        }
    };

    sq_matrix<T>(sq_matrix<T> const& cpy): dim{cpy.dim}, data{cpy.data}{
        if( dim != cpy.dim){
            std::cout<<"Dimension error!"<<std::endl;
            exit(-1);
        }
    }

	//sq_matrix<T>(sq_matrix<T>&&) = default;
	sq_matrix<T>& operator=(sq_matrix<T>&&) = default;
    sq_matrix<T>(sq_matrix<T>&& o) noexcept : dim(std::exchange(o.dim, 0)), data(std::move(o.data)) {}

    T & operator()(int i, int j){return data[dim * i + j];}
    T const& operator()(int i, int j) const {return data[ dim * i + j];}
    T & operator[](int i){return data[i];}
    T const& operator[](int i) const {return data[i];}
    sq_matrix(std::initializer_list<T> const& list, int dimension){
        if(dimension*dimension != static_cast<int>(list.size())){
            std::cout<<"Error: nonsquare matrix!"<<std::endl;
            exit(-1);
        }
        dim = dimension;
        data = list;
    };

    sq_matrix(int dimension, std::initializer_list<T> const& list){
        if(dimension*dimension != static_cast<int>(list.size())){
            std::cout<<"Error: nonsquare matrix!"<<std::endl;
            exit(-1);
        }
        dim = dimension;
        data = list;
    };

    sq_matrix<T>& operator+=(sq_matrix<T> const& m){
        detail::transform_matrix2((*this).data, m.data, (*this).data, add);
        return *this;
    }

    sq_matrix<T>& operator-=(sq_matrix<T> const& m){
        detail::transform_matrix2((*this).data, m.data, (*this).data, sub);
        return *this;
    }

    sq_matrix<T>& operator*=(T const& c){
        detail::transform_matrix1((*this).data, (*this).data, [=](T const& x){return x * c;});
        return *this;
    }
    
    sq_matrix<T>& operator/=(T const& c){
        detail::transform_matrix1((*this).data, (*this).data, [=](T const& x){return x / c;});
        return *this;
    }

    sq_matrix<T>& operator*=(sq_matrix<T> const& m){
        std::vector<T> temp = sq_mat_mul((*this).data, m.data, (*this).dim);        
        (*this).data.swap(temp);
        return *this;
    }
    
    int dimension() const{
		return dim;
	}

    int size() const{
		return dim*dim;
	}
    std::vector<T> read_data() const&{
		return data;
	}

	auto begin(){
		return data.begin();
	}

	auto cbegin() const{
		return data.cbegin();
	}

	auto end(){
		return data.end();
	}

	auto cend() const{
		return data.cend();
    }

    template<typename T1>
    friend std::istream& operator>>(std::istream& s, sq_matrix<T1> & mat);
};

template<typename T>
int sq_mat_prod(sq_matrix<T> & m1, sq_matrix<T> & m2, int lab){
    int n = m1.dimension();
    std::vector<T> temp(n);
    if(lab == 1){
        for(int i = 0; i <  n; i++){
            for(int j = 0; j < n; j++){
                double temp_d  = 0;
                for(int k = 0; k < n; k++){
                    temp_d += m1(i, k) * m2(k, j);
                }
                temp[j] = temp_d;
            }
            for(int j = 0; j < n; j++){
                m1(i, j) = temp[j];
            }
        }
        return 0;
    }
    else if(lab == 2){
        for(int i = 0; i <  n; i++){
            for(int j = 0; j < n; j++){
                double temp_d  = 0;
                for(int k = 0; k < n; k++){
                    temp_d += m1(j, k) * m2(k, i);
                }
                temp[j] = temp_d;
            }
            for(int j = 0; j < n; j++){
                m2(j, i) = temp[j];
            }
        }
        return 0;
    }
    return -1;
}

template<typename T>
sq_matrix<T> operator+(sq_matrix<T> const& m1, sq_matrix<T> const& m2){
    sq_matrix<T> result(m1.dimension());
    detail::transform_matrix2(m1, m2, result, add);
    return result;
}

template<typename T>
sq_matrix<T>&& operator+(sq_matrix<T>&& m1, sq_matrix<T> const& m2){
    detail::transform_matrix2(m1, m2, m1, add);
    return std::move(m1);
}

template<typename T>
sq_matrix<T>&& operator+(sq_matrix<T> const& m1, sq_matrix<T>&& m2){
    detail::transform_matrix2(m1, m2, m2, add);
    return std::move(m2);
}

template<typename T>
sq_matrix<T>&& operator+(sq_matrix<T>&& m1, sq_matrix<T>&& m2){
    detail::transform_matrix2(m1, m2, m1, add);
    return std::move(m1);
}

template<typename T>
sq_matrix<T> operator-(sq_matrix<T> const& m1, sq_matrix<T> const& m2){
    sq_matrix<T> result(m1.dimension());
    detail::transform_matrix2(m1, m2, result, sub);
    return result;
}

template<typename T>
sq_matrix<T>&& operator-(sq_matrix<T>&& m1, sq_matrix<T> const& m2){
    detail::transform_matrix2(m1, m2, m1, sub);
    return std::move(m1);
}

template<typename T>
sq_matrix<T>&& operator-(sq_matrix<T> const& m1, sq_matrix<T>&& m2){
    detail::transform_matrix2(m1, m2, m2, sub);
    return std::move(m2);
}

template<typename T>
sq_matrix<T>&& operator-(sq_matrix<T>&& m1, sq_matrix<T>&& m2){
    detail::transform_matrix2(m1, m2, m1, sub);
    return std::move(m1);
}

template<typename T>
sq_matrix<T> operator*(sq_matrix<T> const& m, T const& c){
    sq_matrix<T> result(m.dimension());
    detail::transform_matrix1(m, result, [c](T const& x){return x * c;});
    return result;
}

template<typename T>
sq_matrix<T>&& operator*(sq_matrix<T>&& m, T const& c){
    detail::transform_matrix1(m, m, [c](T const& x){return x * c;});
    return std::move(m);
}

template<typename T>
sq_matrix<T> operator*(T const& c, sq_matrix<T> const& m){
    sq_matrix<T> result(m.dimension());
    detail::transform_matrix1(m, result, [c](T const& x){return c * x;});
    return result;
}

template<typename T>
sq_matrix<T>&& operator*(T const& c, sq_matrix<T>&& m){
    detail::transform_matrix1(m, m, [c](T const& x){return c * x;});
    return std::move(m);
}

template<typename T1, typename T2>
sq_matrix<T1> operator/(sq_matrix<T1> const& m, T2 const& c){
    sq_matrix<T1> result(m.dimension());
    detail::transform_matrix1(m, result, [c](T1 const& x){return x / c;});
    return result;
}

template<typename T1, typename T2>
sq_matrix<T1>&& operator/(sq_matrix<T1>&& m, T2 const& c){
    detail::transform_matrix1(m.data, m.data, [c](T1 const& x){return x / c;});
    return std::move(m);
}

template<typename T>
sq_matrix<T> operator*(sq_matrix<T> const& m1, sq_matrix<T> const& m2){
    std::vector<T> v(m1.size());
    v = sq_mat_mul(m1.read_data(), m2.read_data(), m1.dimension());
    sq_matrix<T> temp(m1.dimension(), v);
    return temp;
}

template<typename T>
sq_matrix<T>&& operator*(sq_matrix<T> && m1, sq_matrix<T> & m2){
    if(sq_mat_prod(m1, m2, 1) != 0){
        std::cout<<"Matrix product error!"<<std::endl;
    }
    return std::move(m1);
}

template<typename T>
sq_matrix<T>&& operator*(sq_matrix<T> & m1, sq_matrix<T>&& m2){
    if(sq_mat_prod(m1, m2, 2) != 0){
        std::cout<<"Matrix product error!"<<std::endl;
    }
    return std::move(m2);
}

template<typename T>
sq_matrix<T>&& operator*(sq_matrix<T> && m1, sq_matrix<T>&& m2){ 
    if(sq_mat_prod(m1, m2, 1) != 0){
        std::cout<<"Matrix product error!"<<std::endl;
    }
    return std::move(m1);
}

template<typename T>
std::ostream& operator<<(std::ostream& o, sq_matrix<T> const& m){
    o<<m.dimension()<<';';
	int n = {m.dimension()};
		for(int i = 0; i < n; i++){
            for(int j = 0 ; j < n; j++){
                o<<m(i,j)<<',';
            }
//            o<<'\n';
		}
	return o;
}

template <typename T1>
std::istream& operator>>(std::istream& s, sq_matrix<T1>& mat){
    auto rewind = [state = s.rdstate(), pos = s.tellg(), &s](){s.clear(); s.seekg(pos); s.setstate(state);};
    std::string temp_s;
    std::getline(s, temp_s);
    if(!s){rewind(); std::cout<<"Read error!"<<std::endl; return s;}
    std::stringstream ss(temp_s);
    if(!ss){rewind(); std::cout<<"Read error!"<<std::endl; return s;}
    std::getline(ss, temp_s, ';');
    if(static_cast<int>(temp_s.size()) > 0){
        int dim = std::stoi(temp_s);
        std::vector<T1> temp_v;
         for(int i = 0; i < dim * dim; i++){
            std::getline(ss, temp_s, ',');
            if(static_cast<int>(temp_s.size()) > 0){
                std::stringstream temp_ss(temp_s);
                T1 temp;
                temp_ss >> temp;
                temp_v.push_back(temp);
            }
            else{
                rewind();
                std::cout<<"Dimension error!1"<<std::endl;
                return s;
            }
        }
        if(dim * dim == static_cast<int>(temp_v.size())){
            mat.data = std::move(temp_v);
            mat.dim = dim;
        }
        else{
            rewind();
            std::cout<<"Dimension error!2"<<std::endl;
        }
    }
    else{
        rewind();
        std::cout<<"Dimension error!3"<<std::endl;
    }
    return s;
}


template<typename T>
bool sq_matrix_eq(sq_matrix<T> const& m1, sq_matrix<T> const& m2, T eps){
    int n = m1.dimension();
    if(n != m2.dimension() || m1.size() != n*n || m2.size() != n*n){
        return false;
    }
    for(int i = 0; i < n * n; i++){
        if(std::abs(m1[i] - m2[i]) > eps){
            return false;
        }
    }
    return true; 
}
