#include <iostream> //BALLS_AND_BOXESII
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <chrono>
// This program defines a class which is used to simulate the probability distribution of finding fluorescently labeled DNA molecules
// occupying multivalent protein binding sites in TIRF images. The Multicolor_box class allows for a mixture of fluorescently labeled DNA and
// non-fluorescent biotin, both of which can bind to multivalent streptavidin.
// Date written: 7/16/2016
// By Ramsey Kamar
using namespace std;

class Box {
protected:    // Each box is characterized by the number of balls it can hold, and by how many balls it contains.
    int _box_size, _num_balls;
public:
    Box(); //Default constructor, by default box is empty when created. Also by default the size is 3
    Box(int box_size, int num_balls); //Constructor for setting by hand the box size and number of balls
    bool isfull();   //Determine whether a box is full to capacity
    void add_ball();  //Add a ball to a box
    int size_of_box();
    int how_many();  //Tells you how many balls are in a box
};

class Multicolor_Box : public Box{
protected:
    int _num_bright_balls;
    int _num_dark_balls;
public:
    Multicolor_Box(); //Default constructor for derived class just calls default constructor of base class
    Multicolor_Box(int box_size, int num_bright_balls, int num_dark_balls);
    void add_dark_ball();
    void add_bright_ball();
    int how_many_bright();
    int how_many_dark();
};

struct Ball_collection {
protected:
    int _num_bright_balls;
    int _num_dark_balls;
public:
    string remove_ball(); //randomly removes a ball from the collection. Return whether removed ball is "bright" or "dark"
    void populate(int num_bright_balls, int num_dark_balls);
    bool is_collection_empty();
};

int rand_0toN_minus_1(int n); //prototype declaration for choosing a random number from 0 to n - 1

int main(){

    //now we do the real work
    srand(time(NULL)); //Set the seed for random numbers

    int num_boxes;
    int num_balls;
    cout << "Each box can hold three balls. How many boxes would you like? ";
    cin >> num_boxes;
    cout << endl;
    cout << "How many balls would you like? must be less than or equal to " << 3*num_boxes << " :";
    cin >> num_balls; // This is the number of balls you are going to distribute among the boxes
    cout << endl;
    if (num_balls > 3*num_boxes){
        cout << "You can't do that! There aren\'t enough slots in the boxes for that many balls." << endl;
        exit(-1);
    }

    Box *box_collection = new Box[num_boxes];

    int r; //box index which gets randomly chosen
    int occupation_histogram[box_collection[0].size_of_box() + 1] = {0,0,0,0}; //vector containing the occupation numbers of 0,1,2, and 3 balls.




    for (int i = 1; i <= num_balls; i++) {//randomly add balls to the boxes until no balls are left
            r = rand_0toN_minus_1(num_boxes);//randomly choose a box
            if (not(box_collection[r].isfull())) { //if the box you chose isn't full, add a ball to it
                box_collection[r].add_ball();
            } else { //if the first box you chose was full, keep selecting boxes until you find one that is empty and add a ball to it
                while (box_collection[r].isfull()) {
                    r = rand_0toN_minus_1(num_boxes);
                }
                box_collection[r].add_ball();
            }

    }

    //Next we want to find how many boxes have 0 or 1 or 2 or 3 balls
    int n;
    for (int i=0; i < num_boxes; i++){ //Run through the boxes. If a box has n balls in it, add one tick to the histogram for that number.
        n = box_collection[i].how_many();//how many balls in the i_th box
        occupation_histogram[n] += 1;//add a tick to the appropriate number in the histogram
    }

    //Now output histogram to a file so it can be plotted by Origin.
    ofstream histogramfile;
    histogramfile.open("histogram_file.txt");
    histogramfile << "Distributing " << num_balls << " balls among " << num_boxes << " boxes results in the following occupation " << endl;
    histogramfile << "numbers of 0, 1, 2, and 3 balls." << endl;
    for (int i=0; i <= box_collection[0].size_of_box(); i++){
        histogramfile << i << ' ' << occupation_histogram[i] << endl;
    }
    histogramfile << endl;

    //Normalize populations of each ball count so it can be a probability
    histogramfile << "And in terms of percentages: " << endl;
    for (int i = 0; i <= box_collection[0].size_of_box(); i++){
        histogramfile << i << ' ' << static_cast<double>(occupation_histogram[i])/num_boxes << endl;
    }
    histogramfile << endl;
    histogramfile << "Occupation percentage of boxes that have at least one ball: " << endl;
    int nbalob = 0; // Number of boxes with at least one ball.
    for (int i = 1; i <= box_collection[0].size_of_box(); i++){
        nbalob += occupation_histogram[i];
    }
    for (int i = 1; i <= box_collection[0].size_of_box(); i++){
        histogramfile << i << ' ' << static_cast<double>(occupation_histogram[i])/nbalob << endl;
    }

    histogramfile.close();
    delete [] box_collection; //memory cleanup

    Multicolor_Box tstbox; // create a Multicolor_Box object to test out class with.
    cout << endl;
    cout << "Just testing out the Multicolor_Box class here..." << endl;
    cout << "tstbox has " << tstbox.size_of_box() << " slots in it." << endl;
    cout << "tstbox has " << tstbox.how_many() << " total balls in it." << endl;
    tstbox.add_bright_ball();
    cout << "Added a bright ball. How many dark balls? (should be zero) " << tstbox.how_many_dark() << endl;
    tstbox.add_dark_ball();
    cout << "tstbox has " << tstbox.how_many() << " total balls in it." << endl;
    cout << "tstbox has " << tstbox.how_many_bright() << " bright balls in it." <<endl;
    cout << "OK, testing over." << endl << endl;


    //Now create a collection of Multicolor_Box objects
    int num_multi_boxes;
    int num_bright_balls;
    int num_dark_balls;
    cout << "how many multicolor boxes would you like? ";
    cin >> num_multi_boxes;
    cout << endl;
    cout << "how many bright balls would you like? must be less than or equal to " << 3*num_multi_boxes << " : ";
    cin >> num_bright_balls;
    cout << endl;
    cout << "how many dark balls would you like? must be less than or equal to " << 3*num_multi_boxes - num_bright_balls << " : ";
    cin >> num_dark_balls;
    cout << endl;
    if ( num_bright_balls + num_dark_balls > 3*num_multi_boxes){
        cout << "You can't do that! There aren\'t enough slots in the boxes for that many balls." << endl;
        exit(-1);
    }

    Multicolor_Box *box_collection2 = new Multicolor_Box[num_multi_boxes]; //Collection of boxes that can hold dark or light balls.

    Ball_collection bc; // initialize a ball collection
    bc.populate(num_bright_balls,num_dark_balls); //populate ball collection with the bright and dark balls

    string ball_type_removed;
    //OK, now randomly populate the boxes with dark and light balls until no balls are left
    while (!(bc.is_collection_empty())){ //while the ball collection still has balls in it
        ball_type_removed.assign(bc.remove_ball()); //remove a ball and store what kind of ball it is in ball_type_removed
        if (ball_type_removed == "bright"){
            //add a bright ball to the Box Collection
            r = rand_0toN_minus_1(num_multi_boxes);//randomly choose a box
            if (not(box_collection2[r].isfull())) { //if the box you chose isn't full, add a ball to it
                box_collection2[r].add_bright_ball();
            } else { //if the first box you chose was full, keep selecting boxes until you find one that is empty and add a ball to it
                while (box_collection2[r].isfull()) {
                    r = rand_0toN_minus_1(num_multi_boxes);
                }
                box_collection2[r].add_bright_ball();
            }
        } else if(ball_type_removed == "dark"){
            //add a dark ball to box collection
            r = rand_0toN_minus_1(num_multi_boxes);//randomly choose a box
            if (not(box_collection2[r].isfull())) { //if the box you chose isn't full, add a ball to it
                box_collection2[r].add_dark_ball();
            } else { //if the first box you chose was full, keep selecting boxes until you find one that is empty and add a ball to it
                while (box_collection2[r].isfull()) {
                    r = rand_0toN_minus_1(num_multi_boxes);
                }
                box_collection2[r].add_dark_ball();
            }
        }

    }

    //populate histogram of number of boxes with 0,1,2, or 3 bright balls in them
    vector<int> histogram_bright(box_collection2[0].size_of_box()+1);

    for (int i=0; i <= box_collection2[0].size_of_box(); i++){ //zero out bright histogram
        histogram_bright.push_back(0);
    }

    for (int i=0; i < num_multi_boxes; i++){ //Run through the boxes. If a box has n bright balls in it, add one tick to the bright histogram for that number.
        n = box_collection2[i].how_many_bright();//how many bright balls in the i_th box
        histogram_bright[n] += 1;//add a tick to the appropriate number in the histogram
    }

    //populate histogram of number of boxes with 0,1,2, or 3 dark balls in them
    vector<int> histogram_dark(box_collection2[0].size_of_box()+1);

    for (int i=0; i <= box_collection2[0].size_of_box(); i++){ //zero out bright histogram
        histogram_dark.push_back(0);
    }

    for (int i=0; i < num_multi_boxes; i++){ //Run through the boxes. If a box has n dark balls in it, add one tick to the dark histogram for that number.
        n = box_collection2[i].how_many_dark();//how many dark balls in the i_th box
        histogram_dark[n] += 1;//add a tick to the appropriate number in the histogram
    }

    //output bright ball histogram to file
    ofstream bright_histogramfile;
    bright_histogramfile.open("bright_histogram_file.txt");
    bright_histogramfile << "Below are the absolute counts of 0, 1, 2, or 3 bright balls obtained by" << endl;
    bright_histogramfile << "distributing " << num_dark_balls << " dark balls and " << num_bright_balls << " light balls among " << num_multi_boxes << " boxes." << endl ;
    for (int i = 0; i <= box_collection2[0].size_of_box(); i++){
        bright_histogramfile << i << ' ' << histogram_bright[i] << endl;
    }
    bright_histogramfile << endl << endl;

    int nbalobb = 0; // Number Boxes At Least One Bright Ball
    for (int i = 1; i <= box_collection2[0].size_of_box(); i++){
        nbalobb += histogram_bright[i];
    }

    // Now output the normalized histogram for the bright balls.
    bright_histogramfile << "Below are the fractions of visible boxes that are occupied with 1, 2, or 3 bright balls " << endl;
    bright_histogramfile << "obtained by distributing " << num_dark_balls << " dark balls and " << num_bright_balls << " light balls among " << num_multi_boxes << " boxes." << endl ;
    for (int i = 1; i <= box_collection2[0].size_of_box(); i++){
        bright_histogramfile << i << ' ' << static_cast<double>(histogram_bright[i])/nbalobb << endl;
    }

    //output dark ball histogram to file
    ofstream dark_histogramfile;
    dark_histogramfile.open("dark_histogram_file.txt");
    dark_histogramfile << "Below are the absolute counts of 0, 1, 2, or 3 dark balls obtained by" << endl;
    dark_histogramfile << "distributing " << num_dark_balls << " dark balls and " << num_bright_balls << " light balls among " << num_multi_boxes << " boxes." << endl ;
    for (int i = 0; i <= box_collection2[0].size_of_box(); i++){
        dark_histogramfile << i << ' ' << histogram_dark[i] << endl;
    }
    dark_histogramfile << endl;

    // Now output the normalized histogram for the dark balls.

    delete [] box_collection2; // memory cleanup

    return 0;
}

int rand_0toN_minus_1(int n) { //Generate a random number from 0 to number of boxes - 1
    return rand() % n;
}

bool Box::isfull() {
    return (_num_balls >= _box_size);
}

int Box::size_of_box(){
    return _box_size;
}

int Box::how_many() {
return _num_balls;
}

Box::Box(){
    _box_size = 3; _num_balls = 0; //The default number of slots for ball in a box is 3
}

Box::Box(int box_size, int num_balls) {
    _box_size = box_size;
    if (not(num_balls > _box_size)) {
        _num_balls = num_balls;
    }
    else
    { cout << "ERROR! You tried to add " << num_balls << " balls to a box that can than can only hold " << _box_size << " balls." << endl;
    exit(-1);}
}

void Box::add_ball() {
    if (not(isfull())) {
        _num_balls++;
    }
    else
    { cout << "ERROR! You tried to add a ball to a full box." << endl;
    exit(-1);
    }

}

Multicolor_Box::Multicolor_Box(){
    Box{}; //Default constructor of base class would have been called anyways, but
           //I am explicitly calling it because it is good practice.
    _num_bright_balls = 0;
    _num_dark_balls = 0;
}

Multicolor_Box::Multicolor_Box(int box_size, int num_bright_balls, int num_dark_balls){
    _box_size = box_size;
    if ((num_bright_balls + num_dark_balls) <= _box_size) {//check that you're not overfilling the box
        _num_bright_balls = num_bright_balls;
        _num_dark_balls = num_dark_balls;
        _num_balls = _num_bright_balls + _num_dark_balls;
    }
    else{
        cout << "ERROR! You tried to add " << num_bright_balls << " bright balls and " << num_dark_balls << " dark balls to a box that can only hold " << _box_size << " balls." << endl;
        cout << "That is a total of " << num_bright_balls + num_dark_balls << " balls...You can't do that!!" << endl;
        exit(-1);
    }
}

void Multicolor_Box::add_dark_ball(){
    if (not(isfull())) {
        add_ball();
        _num_dark_balls++;
    }
    else
    { cout << "ERROR! You tried to add a dark ball to a full box." << endl;
    exit(-1);
    }
}

void Multicolor_Box::add_bright_ball(){
    if (not(isfull())) {
        add_ball();
        _num_bright_balls++;
    }
    else
    { cout << "ERROR! You tried to add a bright ball to a full box." << endl;
    exit(-1);
    }
}

int Multicolor_Box::how_many_bright(){
    return _num_bright_balls;
}

int Multicolor_Box::how_many_dark(){
    return _num_dark_balls;
}

string Ball_collection::remove_ball(){ //randomly removes a ball from the collection. Return whether removed ball is "bright" or "dark"
    double lb = 0;
    double ub = 1.0;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count(); //set the seed for RNG based on the current time
    static uniform_real_distribution<double> unif(lb, ub);
    static default_random_engine re(seed);
    double r = unif(re);
    cout << "here is a random number between 0 and 1 " << r << endl; //debugging purposes
    cout << "the current ratio of bright to total balls is " << (static_cast<double>(_num_bright_balls)/static_cast<double>(_num_bright_balls + _num_dark_balls)) << endl; //debugging purposes
    if (r <= (static_cast<double>(_num_bright_balls)/static_cast<double>(_num_bright_balls + _num_dark_balls))){
        _num_bright_balls--;
        cout << "bright" << endl; //debugging purposes
        return "bright";
    }else{
        _num_dark_balls--;
        cout << "dark" << endl; //debugging purposes
        return "dark";
    }

}

void Ball_collection::populate(int num_bright_balls, int num_dark_balls){
    _num_bright_balls = num_bright_balls;
    _num_dark_balls = num_dark_balls;
}

bool Ball_collection::is_collection_empty(){
    return (_num_bright_balls + _num_dark_balls == 0);
}


