/*
	Steven Hurley
	svh0040
	StevenHurley@my.unt.edu

	CSCE3444
	Homework 1
	AETG Algorithm
*/


#include<vector>
#include<algorithm>
#include<ctime>
#include<fstream>
using namespace std;

#define SUITES_TO_GENERATE 100
#define CASES_TO_GENERATE 50

vector< vector<int> > generateTuples(int c, int o);

int main()
{
	int components, options; // input

	printf("Enter the number of components (columns): ");
	if( scanf("%d",&components) != 1 )
	{
		printf("FAILED TO READ INTEGER! Aborting...\n");
		exit(EXIT_FAILURE);
	}

	printf("Enter the number of options (rows): ");
	if( scanf("%d",&options) != 1 )
	{
                printf("FAILED TO READ INTEGER! Aborting...\n");
                exit(EXIT_FAILURE);
        }

	// generate our table of tuples
	vector< vector<int> > tupleHash = generateTuples(components,options);

	srand(time(NULL)); // randomization happens here

	vector<int> testCase; // an array of component many integers
	vector< vector<int> > testCaseSet; // 2D array holding our 50 test cases
	testCase.resize(components,-1);

	vector< vector<int> > testSuite; // 2D array containing all best test cases
	vector< vector<int> > bestSuite;
	vector< vector<int> > tupleHashDupe; // a dummy 2D vector for the Hash Table
	bool tuplesRemain; // to determine if we should loop or not

	// Data for Suite Size ------------------------------------
	float avgSuiteSize = 0;
	int bestSuiteSize = 999;
	int worstSuiteSize = 0;
	// --------------------------------------------------------

	clock_t begin = clock(); // start!

	for(int i = 0; i < SUITES_TO_GENERATE; i++)
	{
		tupleHashDupe = tupleHash;
		tuplesRemain = false;
		do {
			testCaseSet.clear(); 
			// generate 50 test cases
			for(int l = 0; l < CASES_TO_GENERATE; l++)
			{
				// pick a random tuple to start with

				int rBucket = rand() % tupleHashDupe.size(); // pick a random bucket

				while(tupleHashDupe.at(rBucket).size() == 0)
					rBucket = (rBucket + 1) % tupleHashDupe.size();
	
				int rIndex = rand() % tupleHashDupe.at(rBucket).size(); // pick a random tuple
							
				testCase.at( tupleHashDupe.at(rBucket).at(rIndex) / options ) = ( tupleHashDupe.at(rBucket).at(rIndex) ); // place tuple in their correct indices
				testCase.at( rBucket / options ) = rBucket;

				/*for(int t = 0; t < components; t++)
				{
					if(testCase.at(t) == -1)
						printf("- ");
					else
						printf("%d ",testCase.at(t));
				}
				printf("\n");*/
						
				// pick the best digit to place in our empty spaces

				int winningVal = 0, score = 0, highScore = 0;
				vector<int> emptyIndices;

				for(int t = 0; t < components; t++) // step through every digit
					if( testCase.at(t) == -1)
						emptyIndices.push_back(t);

				random_shuffle(emptyIndices.begin(),emptyIndices.end()); // shuffle here so we can iterate

				for(int c = 0; c < emptyIndices.size(); c++)
				{
					int index = emptyIndices.at(c);
			
					for(int o = index*options; o < (index+1)*options; o++) // look at all possible options
					{
						for(int v = 0; v < testCase.size(); v++) // step through all of our known digits
						{
							if( testCase.at(v) == -1) // ignore empty digits
								continue;

							// score is calculated here
							if(testCase.at(v) >= (index+1)*options || index == 0) // count how many times later digits appear in our possibleValues
							{
								if(find(tupleHashDupe.at(o).begin(),tupleHashDupe.at(o).end(), testCase.at(v) ) != tupleHashDupe.at(o).end()) // increment score if we find it
									score++;
							}
							else // find how many times possible values appear in previous digits
							{
								if(find(tupleHashDupe.at( testCase.at(v) ).begin(),tupleHashDupe.at( testCase.at(v) ).end(), o) != tupleHashDupe.at( testCase.at(v) ).end()) // increment score if we find it
									score++;
							}		
						}
//						printf("%d - Score: %d\n",o,score); 
						if( score > highScore)
						{
							winningVal = o;
							highScore = score;
						}
						else if( score == highScore && (rand() % 10) < 5)
							winningVal = o;
							score = 0;
					}

					if(highScore == 0)
						testCase.at(index) = rand() % options + (index * options); // pick a random option to fill in
					else
						testCase.at(index) = winningVal;

					highScore = 0;
/*
					for(int t = 0; t < components; t++)
                                        {
       		                                if(testCase.at(t) == -1)
                       	                                printf("- ");
                               		        else
                                                     	printf("%d ",testCase.at(t));
	                                }
                	                printf("\n");*/
				}

				testCaseSet.push_back(testCase);
				fill(testCase.begin(),testCase.end(), -1);
			} // finish generating 50 test cases

/*			for(int t = 0; t < 50; t++)
			{
				printf("\nTest Case #%d: ",t);
				for(int i = 0; i < components; i++)
					printf("%d ",testCaseSet.at(t).at(i));
			}
			printf("\n");*/

			// variables needed for picking winner ---------------------------------------
			int winningTestCase = 0; // index (of 50) of our winning test case
			int highestUncovered = 0; // high score of tuples covered
			int score = 0;		// current score of tuples covered
			// --------------------------------------------------------------------------
			
			// calculate scores for all 50 test Cases
			for(int t = 0; t < CASES_TO_GENERATE; t++)
			{
				// count tuples against uncovered in tupleList
				for( int i = 0; i < components - 1; i++) // first number in tuple
					for( int j = i+1; j < components; j++) // second number in tuple
						for( int k = 0; k < tupleHashDupe.at( testCaseSet.at(t).at(i) ).size(); k++) // scan tuples array for the second number in tuple
							if( tupleHashDupe.at(testCaseSet.at(t).at(i)).at(k) == testCaseSet.at(t).at(j) )
								score++;

//				printf("\nTestCase #%d score: %d ",t,score);
				//finished checking through all the tuples
				if(score > highestUncovered) // found a new winning test Case
				{
					highestUncovered = score;
					winningTestCase = t;
				}
				else if (score == highestUncovered && (rand() % 10) < 5)
					winningTestCase =t;
				score = 0;
			}

			// record the Test Suite and Mark as covered
//			printf("\n\nTestCase #%d won!\n",winningTestCase);
			testSuite.push_back(testCaseSet.at(winningTestCase));
			
			// Delete all the tuples covered by the winning Test Case
			for( int i = 0; i < components; i++) // first number in tuple
                        	for( int j = i+1; j < components; j++) // second number in tuple
                                	for( int k = 0; k < tupleHashDupe.at(testCaseSet.at(winningTestCase).at(i)).size(); k++) // scan tuples array for the second number in tuple
                                        	if( tupleHashDupe.at(testCaseSet.at(winningTestCase).at(i)).at(k) == testCaseSet.at(winningTestCase).at(j) )
							tupleHashDupe.at( testCaseSet.at(winningTestCase).at(i) ).erase(tupleHashDupe.at( testCaseSet.at(winningTestCase).at(i)).begin() + k);

			// check if any Tuples Remain
			tuplesRemain = false;
			for( int i = 0; i < tupleHashDupe.size(); i++)
				if(tupleHashDupe.at(i).size() != 0)
					tuplesRemain = true;

		}while(tuplesRemain);
	
		//record timing and sizes
		avgSuiteSize += testSuite.size(); // add to average
		if(testSuite.size() > worstSuiteSize) // new worst
			worstSuiteSize = testSuite.size();
		if(testSuite.size() < bestSuiteSize) // new best
		{
			bestSuite = testSuite;
			bestSuiteSize = testSuite.size();
		}

		testSuite.clear();

	} //repeat 100 times


	avgSuiteSize /= SUITES_TO_GENERATE;

	clock_t end = clock();

	double elapsedTime = (double(end - begin) / CLOCKS_PER_SEC);

	string filename = "results";
	ofstream out;
	out.open(filename.c_str());

	out <<  bestSuiteSize << endl << endl;
	for (int s = 0; s < bestSuiteSize; s++) 
	{
		for (int c = 0; c < bestSuite.at(s).size(); c++)
			out << bestSuite.at(s).at(c) << " ";
		out << endl;
	}
	out.close();

	printf("\n\x1B[32m===\x1B[33mRESULTS\x1B[32m=====================================V\x1B[0m\n");
	printf("       Time to run : %f seconds\n",elapsedTime);
	printf("Average Suite Time : %f seconds\n\n",elapsedTime/100);

	printf("   Best Suite Size : %d\n",bestSuiteSize);
	printf("    Avg Suite Size : %.0f\n",avgSuiteSize);
	printf("  Worst Suite Size : %d\n",worstSuiteSize);

	printf("\nBest Suite Results is stored in '%s'",filename.c_str());

	printf("\n\x1B[32m=========================================\x1B[33mEND\x1B[32m==*\x1B[0m\n");
}

vector< vector<int> > generateTuples(int c, int o)
{
        vector< vector<int> > hash;
        vector<int> tuples;
        for(int i = 0; i < o*(c - 1); i++) // creates our buckets
                hash.push_back(tuples);

        for(int i = 0; i < o*(c - 1); i++) // fills our buckets with any integers that interact with the index of the bucket
                for(int j = ((i / o) + 1) * o; j < o * c; j++)
                        hash.at(i).push_back(j);

        return hash;
}

