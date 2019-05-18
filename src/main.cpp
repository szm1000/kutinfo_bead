#include "Rational.h"
#include <vector>
#include "matrix.h"
#include "vector2.h"

using namespace std::rel_ops;

bool Rmatrix_eq(const sq_matrix<Rational> &m1, const sq_matrix<Rational> &m2){
    int dim = m1.dimension();
    if(dim != m2.dimension()){
        return false;
    }
    for(int i = 0; i < dim*dim; i++){
        if(m1[i]-m2[i] != Rational{0}){
            return false;
        }
    }
    return true;
}

int main(int, char**) {

    double eps = 1e-10; // Numeric error

    /** Constructor tests **/

    if(true){
        Rational a{};
        Rational b{0};
        Rational c{0,1};
        Rational d{a};
        Rational e{1, -2};
        Rational f{-1, 2};


        if(a!= a && a != b && b!= c && c != d){
            return -1;
        }
        if(a.read_num() != 0 && a.read_den() != 1 && b.read_num() != 0 && b.read_den() != 1){
            return -1;
        }

        try{ 
            a = Rational{0,0}; 
        }
        catch(std::domain_error const &e){
            std::cout<<e.what()<<"\nException handled"<<std::endl;
        }

        //Check simplify()

        if(Rational{6,4} != Rational{3,2}){
            return -1;
        }

        if(Rational{-6,-4} != Rational{3,2}){
            return -1;
        }

        if(e != f){
            return -1;
        }
    }

    /** Assignment operator tests **/

    if(true){
        Rational a{5,13};
        Rational b = a;
        Rational c = a;
        if(a != b){
            return -1;
        }
        a += a;
        if(a != Rational{10,13}){
            return -1;
        }

        c += 17;
        if(c != Rational{226,13}){
            return -1;
        }

        a -= b;
        if(a != Rational{5,13}){
            return -1;
        }
        
        c -= 11;
        if(c != Rational{83,13}){
            return -1;
        }

        a *= b;
        if(a != Rational{25,169}){
            return -1;
        }

        c *= -3;
        if(c != Rational{-249,13}){
            return -1;
        }

        a /= b;
        if(a != Rational{5,13}){
            return -1;
        }

        c /= -5;
        if(c != Rational{-249,-65}){
            return -1;
        }

        /** Comparison operator tests **/

        if(c < b || b > c){
            return -1;
        }

        if(!(c == c) || c!= c){
            return -1;
        }

        if(!(c >= c) || !(c <= c) || c >= (c + 1) || c <= (c - 1)){
            return -1;
        }
    }

    /** Other Function tests **/

    if(true){
        Rational a{5,13};

        if(a.read_num() != 5 && a.read_den() != 13){
            return -1;
        }

        if(a.rec() != Rational{13,5} || a.inv() != Rational{13,5}){
            return -1;
        }
        a.inv();

        if(std::abs(a.double_val() - (5/13.0)) > eps ){
            return -1;
        }
        
        //spec_sqrt test
        
        Rational b{16,25};
        auto c = a.spec_sqrt();
        auto d = b.spec_sqrt();
        double x = 0;
        Rational y{1};
        if(auto sqval = std::get_if<double>(&c)){
            x = *sqval;
        }
        else if(auto sqval2 = std::get_if<Rational>(&c)){
            y = *sqval2;
        }
        if(auto sqval = std::get_if<double>(&d)){
            x = *sqval;
        }
        else if(auto sqval2 = std::get_if<Rational>(&d)){
            y = *sqval2;
        }
        if(std::abs(x - std::sqrt(a.double_val())) > eps){
            return -1;
        }
        if(y != Rational{4,5}){
            return -1;
        }

        //sqrt test

        if(std::abs(sqrt(b) - std::sqrt(b.double_val())) > eps){
            return -1;
        }

        Rational e{5, -2};
        Rational f{-5, 2};

        if(pow(e, 3) != Rational{-125, 8}){
            return -1;
        }

        if(pow(e, 3) != Rational{-125, 8}){
            return -1;
        }

        // newton_sqrt test

        Rational rsq{4,9};
        Rational r1{39791197, 1313197};

        int max_step = 3;
        double sqeps = 1e-5;

        if(std::abs(newton_sqrt(rsq.double_val(), 0.6, max_step, eps) - sqrt(rsq)) > sqeps){
            std::cout<<"Newton_sqrt error"<<std::endl;
            std::cout<<newton_sqrt(rsq.double_val(), 0.6, max_step, eps)<<" "<<sqrt(rsq)<<std::endl;
        }
        if(std::abs(newton_sqrt(r1.double_val(), 5, max_step, eps) - sqrt(r1)) > sqeps){
            std::cout<<"Newton_sqrt error"<<std::endl;
            std::cout<<newton_sqrt(r1.double_val(), 5, max_step, eps)<<" "<<sqrt(r1)<<std::endl;
        }
    }

    /** Operator tests **/

    if(true){
        Rational a{8,29};
        if(a + a != Rational{16, 58} && a - a != Rational{0}){
            return -1;
        }
        if(a + 4 != Rational{124, 29} && a - 1 != Rational{-21,29}){
            return -1;
        }
        if(4 + a != Rational{124, 29} && 1 - a != Rational{21,29}){
            return -1;
        }

        try{ 
            Rational overflow = Rational(std::numeric_limits<int>::max());
            overflow + 1;
        }
        catch(std::overflow_error const &e){
            std::cout<<e.what()<<"\nException handled"<<std::endl;
        }


        if(a * a != Rational{64, 841} && a / a != Rational{1}){
            return -1;
        }
        if(a * 2 != Rational{16, 58} && a / 2 != Rational{4,29}){
            return -1;
        }
        if(2 * a != Rational{16, 58} && 2 / a != Rational{29,4}){
            return -1;
        }
    }
    
    /** Stream operator and consistency tests **/
    if(true){

        // Rmatrix_eq test

        sq_matrix<Rational> test1{2, std::vector{Rational{187,405}, Rational{9,4}, Rational{106,90}, Rational{10,3}}};
        sq_matrix<Rational> test2{2, std::vector{Rational{1,405}, Rational{9,4}, Rational{106,90}, Rational{10,3}}};
        if(!Rmatrix_eq(test1, test1) || Rmatrix_eq(test1, test2)){
            std::cout<<"Error in Rmatrix_eq function!"<<std::endl;
        }

        // Stream operator and consistency with matrix class

        std::stringstream ss1;
        std::stringstream ss2;

        ss1<<2<<';'<<Rational{-5,81}<<','<<Rational{1,-2}<<','<<Rational{2,2}<<','<<Rational{1,3};
        ss1<<'\n'<<2<<';'<<Rational{2,5}<<','<<Rational{7,4}<<','<<Rational{8,45}<<','<<Rational{9,3};
        ss2<<"2; -5/81, -1/2, 2/2, 1/3,";

        sq_matrix<Rational> m1{2};
        sq_matrix<Rational> m2{2};
        sq_matrix<Rational> m3{2};
        ss1>>m1>>m2;
        ss2>>m3;

        sq_matrix<Rational> res{2, std::vector{Rational{137,405}, Rational{5,4}, Rational{106,90}, Rational{10,3}}};
        if(!Rmatrix_eq((m1 + m2), res)){
            std::cout<<"Rational matrix read error!"<<std::endl;
            std::cout<<m1 + m2<<'\n'<<res<<std::endl;
        }
        if(!Rmatrix_eq(m1, m3)){
            std::cout<<"Rational matrix read error!"<<std::endl;
            std::cout<<m1<<'\n'<<m3<<std::endl;
        }

        // Consistency with vector2 class

        vec2<Rational> a{Rational{-1,2}, Rational{5,7}};
        vec2<Rational> b{Rational{7,2}, Rational{9,7}};
        if(std::abs(dot(a, b).double_val() - Rational(163,-196).double_val()) > eps ){
            std::cout<<"Rational vector2 error!"<<std::endl;
            std::cout<<dot(a, b)<<" "<<dot(a, b).double_val()<<" "<<Rational(-163,196).double_val()<<std::endl;
        }
    }

    std::cout<<"Testing is done!"<<std::endl;
    return 0;
}