//
//  Class.cpp
//  object interface
//
//  Created by Holly on 20/11/2013.
//
//

#include "Class.h"
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

// define a base class containing Common parameters(x,y,z,p,Z,A,sizex, sizez, sizey, ecc, order)
class object_t{
public:
    double x;
    double y;
    double z;
    double p;       // Density
    double Z;       // Proton number
    double A;       // Atomic mass number
    double sizex;   // size=diameter for sphere, edge width for rectangle
    double sizey;
    double sizez;
    double ecc;     // Define eccentricity of cigar ends
    double order;
    void set(double ix,double iy,double iz,double ip,double iZ,double iA,double isizex, double isizey, double isizez, double ecc, int order){
        x=ix;
        y=iy;
        z=iz;
        p=ip;
        Z=iZ;
        A=iA;
        sizex=isizex;   // Assumes axis set up inline with parameters - rotation needed
        sizey=isizey;
        sizez=isizez;
        ecc=iecc;
        order=iorder;
    }
    virtual bool inshape(double bx,double by,double bz){
        return false;   // Allows access and redefintion outside class - sphere

        }
};

class sphere_t: public object_t{    //defines a new class for shape sphere
public:
    bool inshape(double bx,double by,double bz){

        double bxc = bx-x;      // Shift everything to centre of coordinate system
        double byc = by-y;
        double bzc = bz-z;
        //cout<<"x,y,z" << x<<","<<y<<","<<z<<endl;
        //cout<< "bx"<<bx << ", " << by << "," << bz << "," << endl;
        //cout<< "bxc"<<bxc << ", " << byc << "," << bzc << "," << endl;
        //cout<< "radius"<<size/2.0 << endl;
        //cout << "sqrt rad"<<sqrt(bxc*bxc + byc*byc + bzc*bzc)<<endl;
        if (bxc*bxc + byc*byc + bzc*bzc <= (sizex/2.0)*(sizex/2.0)) return true; // Just set as sizex for now
        else return false;      // If inside the sphere =true, outside =false
    }
};

class cuboid_t: public object_t{ //defines a class for shape cuboid derived from object_t
public:
    bool inshape(double bx,double by,double bz){
        bxc = bx-x;     // Shift everything to centre of coordinate system
        byc = by-y;
        bzc = bz-z;
        if ((fabs(bxc) - sizex/2 <= 0) && (fabs(byc) - sizey/2 <= 0) && (fabs(bzc) - sizez/2 <= 0)) return true;
        else return false;
    }
};


class cigar_t: public object_t{
public:
    bool inshape(double bx,double by,double bz){    // Defines a cigar with magar axis along y: Assumes size x=size z!!!
        bxc = bx-x;     // Shift everything to centre of coordinate system
        byc = by-y;
        bzc = bz-z;
        if ( (bxc*bxc + (byc/ecc)*(byc/ecc) + bzc*bzc <= (sizex/2.0)*(sizex/2.0)) && (fabs(byc)+ (byc/ecc)/2 - sizey/2 <=0) && (bxc*bxc + bxz*bxz <= (sizex/2)*(sizex/2)) )return true;
        else return false;
    }
};


class material_t{   // Input class returned to main code
public:
    double p;
    double A;
    double Z;
    material_t();
};

material_t::material_t() {  // Take material_t from class
    p=0;
    A=0;
    Z=0;
}


namespace object_paramters {
    std::vector<object_t*> object_list(0);  // Number of different objects in the list
    std::vector<sphere_t> sphere_list(0);   // Each vector contains one kind of shape
}

void initialise();
material_t get_material_properties(double x,double y,double z);

int main(){ //this will be a function
    ofstream myfile;
    myfile.open ("xyzpclass.txt");
    
    //------Definitions------   Does Richard need these???
    double xa=20;   // Set intial grid size
    double ya=20;
    double za=20;
    double E;       // Set intial Energy
    double meansteplength;
    double cs;
    double defect;
    double rotx;    // Define rotation around the x-axis
    double rotz;    // Define rotation arounf the z-axis

    initialise(); //create shapes
    material_t local_parameters; //creates temporary object of type material_t
    
        for (double yinitial = 0; yinitial<ya+1; yinitial ++){
            for (double xinitial =0; xinitial<xa+1; xinitial ++){
                for (double zinitial =0; zinitial<za+1; zinitial ++){
                    local_parameters = get_material_properties(xinitial,yinitial,zinitial); // Need to insert heirachy (order) statements
                    cout<< "density=" << local_parameters.p << endl;
                    if (zinitial==10) myfile << xinitial << "\t"<< yinitial<< "\t" << zinitial<<"\t" << local_parameters.p<<"\t"<< endl;
                }
            }
            myfile<<endl;
        }
    return 0;
}

void initialise(){  //need to be shared - Richard's file will decide which objects are to be created
// create some objects
    double ix;
    double iy;
    double iz;
    double ip;
    double iZ;
    double iA;
    double isizex;
    double isizey;
    double isizez;
    double iecc;
    int iorder;
    

    //std::vector<object_t*> object_list(0); //number of different object in the list
    //std::vector<sphere_t> sphere_list(0); //each vector contains one kind of shape
    sphere_t temp_sphere;
    
    temp_sphere.set(10.0, 10.0, 10.0, 19.2, 196.0, 79.0, 20.0, 20.0, 20.0, 2, 1);//change // Richard needs external access to this - new shared namespace "yeti" header file the same in all code bits allows you to share, declared everywhere, but only defined once.
    object_parameters::sphere_list.push_back(temp_sphere); //increases size of array by one, copies into end
    object_parameters::object_list.push_back(&object_parameters::sphere_list[0]); //saving address in object list
    
    /*temp_sphere.set(ix, iy, iz, ip, iZ, iA, isizex, isizey, isizez, iecc, iorder);//change
    object_parameters::sphere_list.push_back(temp_sphere); //increases size of array by one, copies into end
    object_parameters::object_list.push_back(&object_parameters::sphere_list[0]); //saving address in object list
    */
    
    
    temp_sphere.set( ix, iy, iz, ip, iZ, iA, isizex, isizey, isizez, ecc, order);
    object_parameters::sphere_list.push_back(temp_sphere);

    return;
}

material_t get_material_properties(double x,double y,double z){
    //determine pAZ from inshape

    
            material_t result;

            //for(int i=0; i<object_list.size(); i++){
        
   
            bool test = object_parameters::object_list[0]->inshape(x,y,z);
            if (test == true){
                result.p = object_parameters::object_list[0]->p;
                //cout<< "p=" << object_parameters::object_list[0]->p <<endl;
            }else if (test == false){
                ;// Set as order 0 density???
            }
            // }
        return result; //passes all variables contained here to main code.
    
}

// Need to work out how the order / heirachy are progressing
// how shape orientation in relation to axis is going to change the inshape coding
// which parts are going to be in my code, which in Richard's, which in Sarah's.


            
    //check the parameter are sensible - Richard's input file
    //if ((x*x <= arrayx*arrayx) && (y*y <= arrayy*arrayy) && (z*z <= arrayz*arrayz)){}
    //code to detect if you have two objects overlapping - print a warning that they intersect


    /* objectlist(o).x or .y or .z
     temp_object.set(ix,iy,iz...) to load variables into
     objectlist.push_back(temp_object); -> pass in temp object
     creates an object, adds it to the end of the array
     increaes size of array by one set contents to temp_object
     int_array.push_back(4) */
     


//code to detect if you have two objects overlapping - print a warning that they intersect
