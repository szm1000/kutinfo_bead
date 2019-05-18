#include <iostream>
#include <numeric>
#include <stdexcept>
#include <cmath>
#include <variant>
#include <utility>
#include <string>
#include <sstream>
#include <limits>

template<typename T>
T sq(T a){
    return a*a;
}

class Rational;
double sqrt(Rational const &r);

class Rational{

    /***Numerator and denominator***/

    int num;
    int denom;

    /*** Simplify to prevent overflow***/

    constexpr Rational& simplify(){
        int gcd = std::gcd(num, denom);
        if(gcd != 0){
            num /= gcd;
            denom /= gcd;
        }
        if( num <= 0 && denom < 0 ){
            num *= -1;
            denom *= -1;
        }
        else if(num >= 0 && denom < 0){
            num *= -1;
            denom *= -1;
        }
        return *this;
    };
    
    public:

    /*** Constructors ***/

    constexpr Rational(int a): num{a}, denom{1}{}

    //Defalut constructor

    constexpr Rational(): Rational{0}{}

    constexpr Rational(int a, int b): num{a}, denom{b}{
        if(denom == 0){
            throw std::domain_error{"Exception: zero denominator"};
        }
        simplify();
    }

    //Copy constructor
    
    Rational(const Rational& cpy){
        num = cpy.num; 
        denom = cpy.denom;
    }

    /*** Assignment operators ***/

    constexpr Rational& operator=(const Rational& r){
        num = r.num; 
        denom = r.denom; 
        return *this;
    }

    constexpr Rational& operator+=(const Rational& r){
        int lcm = std::lcm(denom, r.denom);     //Possible overflow
        int num1 = num * lcm / denom;           //Possible overflow
        int num2 = r.num * lcm / r.denom;       //Possible overflow
        if(num2 > 0 && std::numeric_limits<int>::max() - num2 < num1){
            throw std::overflow_error{"Exception: int overflow"};
        }
        else if(num2 < 0 && std::numeric_limits<int>::min() - num2 > num1){
            throw std::overflow_error{"Exception: int overflow"};
        }
        num = num1 + num2;
        denom = lcm;
        return simplify();
    }

    constexpr Rational& operator-=(const Rational& r){
        int lcm = std::lcm(denom, r.denom);     //Possible overflow
        int num1 = num * lcm / denom;           //Possible overflow
        int num2 = lcm * r.num / r.denom;       //Possible overflow
        if(num2 > 0 && std::numeric_limits<int>::min() + num2 > num1){
            throw std::overflow_error{"Exception: int overflow"};
        }
        else if(num2 < 0 && std::numeric_limits<int>::max() + num2 < num1){
            throw std::overflow_error{"Exception: int overflow"};
        }
        num = num1 - num2;
        denom = lcm;
        return simplify();
    }

    constexpr Rational& operator*=(const Rational& r){
        num *= r.num;                           //Possible overflow
        denom *= r.denom;                       //Possible overflow
        return simplify();
    }

    constexpr Rational& operator/=(const Rational& r){
        if(r.num == 0){
            throw std::domain_error{"Exception: division by zero"};
        }
        num *= r.denom;                         //Possible overflow
        denom *= r.num;                         //Possible overflow
        return simplify();
    }

    /*** Other functions ***/

    constexpr int read_num() const& {
        return num;
    }

    constexpr int read_den() const& {
        return denom;
    }

    constexpr Rational rec() const&{
        return Rational(denom, num);
    }

    Rational& inv(){
        std::swap(num, denom);
        return *this;
    }

    constexpr double double_val() const& { 
        return num/static_cast<double>(denom);
    }

    // spec_sqrt can return Rational type if possible

    using sqrt_type = std::variant<double, Rational>;
    sqrt_type spec_sqrt(){
        if((*this).double_val() < 0){
            throw std::domain_error{"Exception: result is complex"};
        }
        int cast_num = static_cast<int>(std::sqrt(num) + 0.1);
        int cast_den = static_cast<int>(std::sqrt(denom) + 0.1);
        if(sq(cast_num) == num && sq(cast_den) == denom){
           return Rational{cast_num, cast_den};
        }
        else{
            return sqrt(*this);
        }
    }
};

/*** Operators ***/

Rational operator+(const Rational& r1, const Rational& r2){
    Rational result = r1;
    result += r2;
    return result; 
}

Rational operator+(const Rational& r, const int & c){
    Rational result(c);
    result += r;
    return result; 
}

Rational operator+(const int& c, const Rational& r){
    Rational result(c);
    result += r;
    return result; 
}

Rational operator-(const Rational& r1, const Rational& r2){
    Rational result = r1;
    result -= r2;
    return result; 
}

Rational operator-(const Rational& r, const int & c){
    Rational result(-1*c);
    result += r;
    return result; 
}

Rational operator-(const int& c, const Rational& r){
    Rational result(c);
    result -= r;
    return result; 
}

Rational operator*(const Rational& r1, const Rational& r2){
    Rational result = r1;
    result*= r2;
    return result; 
}

Rational operator*(const Rational& r, const int& c){
    Rational result(c);
    result *= r;
    return result; 
}

Rational operator*(const int& c, const Rational& r){
    return r*c;                                 //Posible owerflow
}

Rational operator/(const Rational& r1, const Rational& r2){
    Rational result = r1;
    result/= r2;
    return result; 
}

Rational operator/(const Rational& r, const int& c){
    Rational result(r);
    result /= Rational(c);
    return result; 
}

Rational operator/(const int& c, const Rational& r){
    Rational result{c};
    result/= r;
    return result; 
}

// Comparsion operators


bool operator==(const Rational &r1, const Rational &r2){
    return r1.read_num() == r2.read_num() && r1.read_den() == r2.read_den();
}

bool operator<(const Rational &r1, const Rational &r2){
    return r1.double_val() < r2.double_val();
}

/*
bool operator!=(const Rational& r1, const Rational& r2){
    return !(r1 == r2);
}

bool operator>(const Rational& r1, const Rational& r2){
    return !(r1 < r2)
}

bool operator<=(const Rational& r1, const Rational& r2){
    return r1 < r2 || r1 == r2;
}

bool operator>=(const Rational& r1, const Rational& r2){
    return r1 > r2 || r1 == r2;
}
*/

std::ostream& operator<<(std::ostream& o, Rational const &r){
        o<<r.read_num()<<"/"<<r.read_den()<<" ";
    return o;
}

std::istream& operator>>(std::istream& istr, Rational &r){
    auto rewind = [state = istr.rdstate(), pos = istr.tellg(), &istr](){istr.clear(); istr.seekg(pos); istr.setstate(state);};
    int n = 0;
    int d = 1;
    std::string temp;
    if(std::getline(istr, temp, ',')){
        std::stringstream ss(temp);
        std::getline(ss, temp, '/');
        if(!ss){rewind(); std::cout<<"Read error!"<<std::endl; return istr;}
        n = std::stoi(temp);
        std::getline(ss, temp, ' '); 
        if(!ss){rewind(); std::cout<<"Read error!"<<std::endl; return istr;}
        d = std::stoi(temp);
    }
    else{
        std::cout<<"Read error!"<<std::endl;
        rewind();
        return istr;
    }
    r = Rational{n, d};
    return istr;
}

double sqrt(Rational const &r){        
    if(r.double_val() < 0){
        throw std::domain_error{"Exception: result is complex"};
    }
    return std::sqrt(r.double_val());
}

double newton_sqrt(Rational& r){        
    if(r.double_val() < 0){
        throw std::domain_error{"Exception: result is complex"};
    }
    return std::sqrt(r.double_val());
}


Rational pow(const Rational& r, const int& a){
    auto powint = [&](int p, int a){return static_cast<int>(std::pow(p, a));};
    double dbv = r.double_val();
    int exp = std::abs(a);
    if(dbv > 0 && a > 0 && std::pow(std::numeric_limits<int>::max(), 1.0/a) > dbv){
        throw std::overflow_error{"Exception: int overflow"};
    }
    else if(dbv < 0 && a > 0 && a % 2 != 0 && -std::pow(std::numeric_limits<int>::min(), 1.0/a) > -dbv){ //-std::numeric_limits<int>::min() cannot be used
        throw std::overflow_error{"Exception: int overflow"};
    }
    Rational res = Rational(powint(r.read_num(), exp), powint(r.read_den(), exp));
    if(a < 0){
            res.inv();
        }
    return res;
}

double pow(const Rational& r, const double& a){
    return std::pow(r.double_val(), a);
}

Rational abs(const Rational& r){
    return Rational(std::abs(r.read_num()), std::abs(r.read_den()));
}

double newton_sqrt(double x_x, double x0, int max_step, double epsilon){
    auto f = [&](double x){ return x * x - x_x;};
    auto f_der = [&](double x){ return 2.0 * x;};
    auto check = [&](double a, double b ){ return std::abs(a - b) > epsilon;};
    double prev = 0;
    int counter = 0;
    while(check(prev,x0) && counter < max_step){
        prev = x0;
        x0 = x0 - f(x0) / f_der(x0);
        counter++;
    }
    return x0;
}