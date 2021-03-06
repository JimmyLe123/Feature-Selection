//Implemented the Forward Selection, but output format is not clean at all, and still need backwards elimination. 
#include <iostream>
#include <string>
//The following 3 include files are for rand(). Source: (https://www.cplusplus.com/reference/cstdlib/rand/)
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//Stream class to read and write from files. Source:(https://www.cplusplus.com/doc/tutorial/files/)
#include <fstream>
//We need to include the math class to use the power and square root functions
#include <math.h>
//The following two lines are needed for the built in timer function. Source:(https://www.geeksforgeeks.org/measure-execution-time-function-cpp/)
#include <chrono>
//The following two lines are for outputting the array without loops(https://www.techiedelight.com/print-contents-array-cpp/)
#include <algorithm>
#include <vector>
using namespace std::chrono;
using namespace std;


float accuracy = 0;
int algorithmChoice;
float best_accuracy_so_far = 0;
double Distance;
string File_Choice;
float label_object_to_classify;
double nearest_neighbor_distance;
float nearest_neighbor_label;
int nearest_neighbor_location;
float number_correctly_classified = 0;
double object_to_classify;

//Checking to see if current feature is already inside of set for the cross validation function
bool check_if_feature_is_already_in_set(int current_set_of_features[300], int j)
{
	for (int i = 0; i < 300; i++)
	{
		if (j == current_set_of_features[i])
		{
			return true;
		}
	}

	return false;
}

//Make sure that the feature at current level and 'k' value in loop are equal.
bool check_if_feature_is_the_one_Im_adding(int feature_to_add_at_this_level, int k)
{
	if (feature_to_add_at_this_level == k)
		return false;
	else
		return true;
}

//This Function is purely for the intersect portion of the search.
bool checkIntersection(int current_set_of_features[], int k)
{
	int check = 0;
	//Checking to see if 'k' value was inside of current_set_of_features
	for (int i = 0; i < 300; i++)
	{
		if (k == current_set_of_features[i])
		{
			check++;
		}
	}
	//Check == 0 means that 'k' doesnt appear inside of current set of features
	if (check == 0)
	{
		return true;
	}
	else
		return false;

}

//Put feature into consideration.
int* pushback(int* current, int feature) {
	int i = 0;
	while (true) {
		if (current[i] == 0) {
			current[i] = feature;
			return current;
		}
		++i;
	}
}

//Once done with feature, then throw it away.
int* discard(int* current, int feature) {
	int i = 0;
	while (true) {
		if (current[i] == feature) {
			//cout << "Found " << feature << endl;
			current[i] = 0;
			return current;
		}
		++i;
	}
}

//This is trying to print out the array in one line. Source(https://www.techiedelight.com/print-contents-array-cpp/)
void print(const int& i) {
	std::cout << i << ' ';
}

float leave_one_out_cross_validation(double data[502][102], int current_set_of_features[], int feature_to_add_at_this_level)
{
	number_correctly_classified = 0;
	//Add feature that is being considered
	current_set_of_features = pushback(current_set_of_features, feature_to_add_at_this_level);
	//The whole purpose of the 3 for loops is iterating all of the rows and columns for the certain feature. We also want to make sure that we do not calculate the distance for the same feature.
	for (int i = 0; i < 300; i++)
	{
		label_object_to_classify = data[i][0];	//label_object_to_classify = data(i,1);
		nearest_neighbor_distance = 1000000;
		nearest_neighbor_location = 1000000;
		for (int k = 0; k < 300; k++)
		{
			//Make sure that you do not calculate distance with yourself twice
			if (k != i)
			{
				//Reset distnace between differnet iterations(Just in case)
				Distance = 0;
				for (int j = 1; j < 11; j++)
				{
					if (check_if_feature_is_already_in_set(current_set_of_features, j))
					{
						object_to_classify = data[i][j];
						Distance += (pow((object_to_classify - data[k][j]), 2));
					}
				}
				Distance = sqrt(Distance);
				//If distance is less than the lowest neighbor distance, than set the nearest neighbor stuff to current iteration.
				if (Distance < nearest_neighbor_distance)
				{
					nearest_neighbor_distance = Distance;
					nearest_neighbor_location = k;
					nearest_neighbor_label = data[nearest_neighbor_location][0];		//nearest_neighbor_label = data(nearest_neighbor_location,1);
				}
			}
		}
		if (label_object_to_classify == nearest_neighbor_label)
		{
			//cout << "Match at location: " << nearest_neighbor_location << endl;
			number_correctly_classified = number_correctly_classified + 1;
		}
	}
	accuracy = number_correctly_classified / 300;
	cout << "accuracy = " << accuracy * 100 << "%" << endl;

	//Remove feature that is being considered when finished
	current_set_of_features = discard(current_set_of_features, feature_to_add_at_this_level);
	return accuracy;
}

void feature_search_demo(double data[502][102])
{
	int current_set_of_features[300] = {}; //Initialize empty set, or array in this case.
	int set_features_dummy[10] = {};	   //Dummy Array to hold the set that has the highest accuracy.
	float highest_Accuracy = 0;			   //Take the highest accuracy
	//Small(i = 0-300, k = 1-11), Large(i = 0-500, k = 1-101), Range of large and small data set
	for (int i = 0; i < 300; i++)
	{
		cout << "On the " << to_string(i + 1) << "th level of the search tree" << endl;
		//Hold features to add
		int feature_to_add_at_this_level[300] = {};
		//Compare with accuracy that is going to be calculated later.
		best_accuracy_so_far = 0;
		for (int k = 1; k < 11; k++)
		{
			if (checkIntersection(current_set_of_features, k)) //Check to see if current 'k' feature is already inside of current set of features.
			{
				cout << "--Considering adding the " << to_string(k) << " feature" << "(" << data[i][k] << ")" << endl;
				accuracy = leave_one_out_cross_validation(data, current_set_of_features, k);
				//Keep track of highest accuracy;
				if (accuracy > best_accuracy_so_far)
				{
					//record at every single level which one has the best accuracy.
					//record current set of features with that accuracy.
					best_accuracy_so_far = accuracy;
					if (highest_Accuracy < accuracy)
					{
						highest_Accuracy = accuracy;
					}
					feature_to_add_at_this_level[i] = k;
				}
			}
		}
		//Say we add feature 1, then we have {1} with ...% accuracy
		//Say we add feature 4, then we have {1,4} with ...% accuracy(combined)
		//Add in current feature to add inside of array(set).
		current_set_of_features[i] = feature_to_add_at_this_level[i];	//current_set_of_features = feature_to_add_at_this_level;

		memcpy(set_features_dummy, current_set_of_features, 10);
		cout << "On level " << to_string(i+1) << " I added feature " << to_string(feature_to_add_at_this_level[i]) << " to current set" << endl;
		cout << "Best Accuracy On this Level is " << best_accuracy_so_far*100 << "%" << " for feature " << feature_to_add_at_this_level[i] << endl;
		cout << "The set of the highest accuracy so far is: ";
		for_each(begin(set_features_dummy), end(set_features_dummy), print);
		cout << endl;
	}
	//Convert Accuracy to percentage and output the final set that had the highest accuracy
	cout << "The set had the highest accuracy below: " << endl;
	cout << "{" << for_each(begin(set_features_dummy), end(set_features_dummy), print) << "}" << " with highest accuracy = " << highest_Accuracy *100 << "%" << endl;
}

void feature_search_demo_backward(double data[502][102])
{
	int current_set_of_features[300] = {}; //Initialize empty set, or array in this case.
	for (int a = 0; a < 300; a++)
	{
		for (int b = 1; b < 11; b++)
		{
			current_set_of_features[a] = data[a][b];
		}
	}	
	int set_features_dummy[10] = {};						   //Dummy Array to hold the set that has the highest accuracy.
	float highest_Accuracy = 0;
	//Small(i = 0-300, k = 1-11), Large(i = 0-500, k = 1-101), Range of large and small data set
	for (int i = 0; i < 300; i++)
	{
		cout << "On the " << to_string(i+1) << "th level of the search tree" << endl;
		//Hold features to add
		int feature_to_add_at_this_level[300] = {};
		//Compare with accuracy that is going to be calculated later.
		best_accuracy_so_far = 0;
		for (int k = 1; k < 11; k++)
		{
			if (checkIntersection(current_set_of_features, k)) //Check to see if current 'k' feature is already inside of current set of features.
			{
				cout << "--Considering removing the " << to_string(k) << " feature" << "(" << data[i][k] << ")" << endl;
				accuracy = leave_one_out_cross_validation(data, current_set_of_features, k);
				//Keep track of highest accuracy;
				if (accuracy > best_accuracy_so_far)
				{
					//record at every single level which one has the best accuracy.
					//record current set of features with that accuracy.
					best_accuracy_so_far = accuracy;
					if (highest_Accuracy < accuracy)
					{
						highest_Accuracy = accuracy;
					}
					feature_to_add_at_this_level[i] = k;
				}
			}
		}
		//Say we add feature 1, then we have {1} with ...% accuracy
		//Say we add feature 4, then we have {1,4} with ...% accuracy(combined)
		//Add in current feature to add inside of array(set).
		current_set_of_features[i] = feature_to_add_at_this_level[i];	//current_set_of_features = feature_to_add_at_this_level;
		memcpy(set_features_dummy, current_set_of_features, 10);
		cout << "On level " << to_string(i + 1) << " I removed feature " << to_string(feature_to_add_at_this_level[i]) << " to current set" << endl;
		cout << "Best Accuracy On this Level is " << best_accuracy_so_far * 100 << "%" << " for feature " << feature_to_add_at_this_level[i] << endl;
	}
	//Convert Accuracy to percentage and output the final set that had the highest accuracy
	cout << "The set had the highest accuracy below: " << endl;
	//"for_each()...,print)" is for outputting the array without outputting. Source(https://www.techiedelight.com/print-contents-array-cpp/).
	cout << "{" << for_each(begin(set_features_dummy), end(set_features_dummy), print) << "}" << " with highest accuracy = " << highest_Accuracy * 100 << "%" << endl;
}

int main(void)
{
	//Need to call seed only once.
	//srand(time(NULL));
	//This string is for the specific file
	string readLine;
	//Max size of the data set(Formatted like this for indexing purposes)
	double data[502][102] = {};
	cout << "Welcome to Jimmy Le's Feature Selection Algorithm." << endl;
	cout << "Type in the name of the file to test: ";
	cin >> File_Choice;
	//ifstream is the class to read from files
	ifstream read_File(File_Choice);
	//Putting values from data file into a 2D array
	//Reading text from file into array. Source(https://stackoverflow.com/questions/36708370/reading-from-txt-file-into-two-dimensional-array-in-c)
	if (File_Choice == "CS170_SMALLtestdata__30.txt" || "CS170_small_special_testdata__95.txt")
	{
		for (int i = 0; i < 300; i++)
			for (int j = 0; j < 11; j++)
				read_File >> data[i][j];
	}
	else if (File_Choice == "CS170_largetestdata__29.txt")
	{
		for (int i = 0; i < 500; i++)
			for (int j = 0; j < 101; j++)
				read_File >> data[i][j];
	}
	else
	{
		cout << "Invalid Input";
		return 0;
	}
	//Select Forward/Backward search
	cout << "Type the number of the algorithm you want to run." << endl;
	cout << "1) Forward Selection" << endl;
	cout << "2) Backward Elimination" << endl;
	cin >> algorithmChoice;
	if (algorithmChoice == 1)
	{
		cout << "You have chosen the forward selection" << endl;
		//Start time
		auto start = high_resolution_clock::now();
		feature_search_demo(data);
		//End time
		auto stop = high_resolution_clock::now();
		//Calculate how much time it took for search to run
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "Time taken for program: " << duration.count() << " microseconds." << endl;
	}
	else if (algorithmChoice == 2)
	{
		cout << "You have chosen the backwards elimination" << endl;
		//Start time
		auto start = high_resolution_clock::now();
		feature_search_demo_backward(data);
		//End time
		auto stop = high_resolution_clock::now();
		//Calculate how much time it took for search to run
		auto duration = duration_cast<microseconds>(stop - start);
		cout << "Time taken for program: " << duration.count() << " microseconds." << endl;
	}
	else
	{
		cout << "Invalid Input" << endl;
		return 0;
	}
	return 0;
}
