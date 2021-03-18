//Search function done, but still need to implement cross validation

#include <iostream>
#include <string>
//The following 3 include files are for rand(). Source: (https://www.cplusplus.com/reference/cstdlib/rand/)
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
//Stream class to read and write from files. Source:(https://www.cplusplus.com/doc/tutorial/files/)
#include <fstream>	
#include <numeric>
//We need to include the math class to use the power and square root functions
#include <math.h>
using namespace std;

int algorithmChoice;
float accuracy = 0;
float best_accuracy_so_far = 0;
float number_correctly_classified = 0;
float object_to_classify;
float label_object_to_classify;
float nearest_neighbor_distance;
int nearest_neighbor_location;
float nearest_neighbor_label;
float Distance;
string File_Choice;

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

float leave_one_out_cross_validation(float data[502][102], int current_set_of_features[], int feature_to_add_at_this_level)
{
	accuracy = rand();
	//cout << "Random Number = " << accuracy << endl;
	return accuracy;
	/*Real Code from slides*/
	number_correctly_classified = 0;
	for (int i = 0; i < 300; i++)
	{
		//Make equal to features
		//object_to_classify = data[i][k];		//object_to_classify = data(i,2:end);
		//Make equal to class label
		label_object_to_classify = data[i][1];	//label_object_to_classify = data(i,1);
		//Setting both nearest neighbor variables to be "infinite"
		//I.e, give it an unreasonably large number to make it "infinite"
		nearest_neighbor_distance = 1000000;
		nearest_neighbor_location = 1000000;
		for(int k = 0; k < 300; k++)
		{
			if(k != i)
			{
				object_to_classify = data[i][k];		//object_to_classify = data(i,2:end);
				for(int j = 1; j < 11; i++)
				{
					//if (j is inside current set of features || if k is the feature that I am adding)
					//{
					//	Distance += (pow((object_to_classify - data[k][j]), 2));		//distance = sqrt(sum((object_to_classify - data(k,2:end)).^2));	
					//}
				}
				Distance = sqrt(Distance);
				if (Distance < nearest_neighbor_distance)
				{
					nearest_neighbor_distance = Distance;
					nearest_neighbor_location = k;
					nearest_neighbor_label = data[nearest_neighbor_location][1];		//nearest_neighbor_label = data(nearest_neighbor_location,1);
				}
			}
		}
		if(label_object_to_classify == nearest_neighbor_label)
		{
			number_correctly_classified = number_correctly_classified + 1;
		}

	}
	accuracy = number_correctly_classified/300;
}

void feature_search_demo(float data[502][102])
{
	int current_set_of_features[300] = {}; //Initialize empty set
	//Small(i = 0-300, k = 1-11), Large(i = 0-500, k = 1-101)
	for (int i = 0; i < 300; i++)
	{
		cout << "On the " << to_string(i+1) << "th level of the search tree" << endl;
		int feature_to_add_at_this_level = {};
		best_accuracy_so_far = 0;
		for (int k = 1; k < 11; k++)
		{
			if (checkIntersection(current_set_of_features,k)) //Only consider adding, if not already added. Check to see if current 'k' feature is already inside of current set of features.
			{
				cout << "--Considering adding the " << to_string(k) << " feature" << "( " << data[i][k] << ")"<< endl;
				accuracy = leave_one_out_cross_validation(data, current_set_of_features, k+1);
				if (accuracy > best_accuracy_so_far)
				{
					best_accuracy_so_far = accuracy;
					feature_to_add_at_this_level = k;
				}
			}
		}
		current_set_of_features[i] = feature_to_add_at_this_level;	//current_set_of_features = feature_to_add_at_this_level;
		cout << "On level " << to_string(i+1) << " I added feature " << to_string(feature_to_add_at_this_level) << " to current set" << endl;
	}
	//cout << "Accurcay = " << accuracy << endl;
	cout << "End of Search" << endl;
}

int main(void)
{
	//Need to call seed only once.
	srand(time(NULL));
	//This string is for the specific file
	string readLine;
	//Max size of the data set(Formatted like this for indexing purposes)
	float data[502][102] = {};
	cout << "Welcome to Jimmy Le's Feature Selection Algorithm." << endl;
	cout << "Type in the name of the file to test: ";
	cin >> File_Choice;
	//ifstream is the class to read from files
	ifstream read_File(File_Choice);
	//Putting values from data file into a 2D array
	//Reading text from file into array. Source(https://stackoverflow.com/questions/36708370/reading-from-txt-file-into-two-dimensional-array-in-c)
	if (File_Choice == "CS170_SMALLtestdata__30.txt")
	{
		for (int i = 0; i < 300; i++)
			for (int j = 0; j < 11; j++)
				read_File >> data[i][j];
	}
	else if (File_Choice == "CS170_largetestdata__29.txt")
	{
		for (int i = 1; i < 501; i++)
			for (int j = 1; j < 102; j++)
				read_File >> data[i][j];
	}
	//Select Forward/Backward search
	cout << "Type the number of the algorithm you want to run." << endl;
	cout << "1) Forward Selection" << endl;
	cout << "2) Backward Elimination" << endl;
	cin >> algorithmChoice;
	if (algorithmChoice == 1)
	{
		cout << "You have chosen the forward selection" << endl;
		feature_search_demo(data);
	}
	else if (algorithmChoice == 2)
	{
		cout << "You have chosen the backwards elimination" << endl;
		feature_search_demo(data);
	}
	else
	{
		cout << "Invalid Input" << endl;
		return 0;
	}
	return 0;
}
