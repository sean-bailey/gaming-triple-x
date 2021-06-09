#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <random>
#include <algorithm>
#include <functional>
#include <list>
#include <sstream>
#include <iterator>


using namespace std;

/*

Let's make a game which starts with a single player which can 
determine the correct combo to hack in to a terminal.

Add in hints, probes, tools the "hacker" can use.

Add in detection/enemies/time limit/consequenses

Take this concept and evolve it to a red-team blue-team game

*/


string GetAgentName(int levelset)
{
    string AgentType;
    switch(levelset)
        {
            case 0 :
            AgentType="intern";
            break;

            case 1 :
            AgentType="agent";
            break;

            case 2 :
            AgentType="special agent";
            break;
        }
        return AgentType;

}

float getRatio(std::vector<int> const &input, int index1, int index2){

    return (float)input[index2] / (float)input[index1];

}

int getSum(std::vector<int> const &input, int index1, int index2){

    return input[index2] + input[index1];
}

int getProduct(std::vector<int> const &input, int index1, int index2)
{
    return input[index2]*input[index1];
}

int getTotalSum(std::vector<int> const &input){
    int sum_of_elems=0;
    for (auto& n : input)
        sum_of_elems += n;

    return sum_of_elems;

}
int getDifference(std::vector<int> const &input, int index1, int index2){

    return input[index2] - input[index1];
}


//we'll need a function which generates a random array of numbers 
// based on the difficulty level.
//we don't want to use arrays. We want to use vectors.
//https://www.cplusplus.com/reference/vector/vector/
// two methods here: https://stackoverflow.com/questions/21516575/fill-a-vector-with-random-numbers-c
vector<int> GetProblemSet(int levelset){
    int arraylength = (levelset + 1) * 3;
    int maxvalue = arraylength * 10;
    // First create an instance of an engine.
    random_device rnd_device;
    // Specify the engine and distribution.
    mt19937 mersenne_engine {rnd_device()};  // Generates random integers
    uniform_int_distribution<int> dist {1, maxvalue};

    auto gen = [&dist, &mersenne_engine](){
                   return dist(mersenne_engine);
               };
    
    vector <int> returnVector(arraylength);
    generate(begin(returnVector),end(returnVector),gen);
    
    /*
    for (auto i : returnVector) {
        cout << i << " ";
    }
    */
    return returnVector;
    

    //static int r[arraylength()];

}

//C++ is frustratingly different than Python... It needs a LOT of work to
//do simple things with array-like objects
void vectorPrint(std::vector<int> const &input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i) << ' ';
    }
}

void listPrint(std::list<std::string> const &list)
{
    for (auto const &i: list) {
        std::cout << i << std::endl;
    }
}
//we need to give a randomized hint based on the information
//perhaps hints should be based on the level? 
string giveHint(std::vector<int> const &input,int levelset)
{
    int mindifficulty=15;
    srand (time(NULL));
    int maxrand=(levelset+1)*10;
    if (maxrand < mindifficulty){
        maxrand=mindifficulty;
    }
    //make a switch statement which favors more varied hints
    // for lower numbers. Logically, the greater your number range,
    // the less likely you'll land on these lower number ranges
    int hintnumber = rand() % maxrand + 1;
    //my compiler cannot handle range for switches. I'll use if statements.
    //so at medium difficulty, you have a 20% chance of the probe revealing nothing
    //at high difficulty, you have a 50% chance of the probe revealing nothing.
    string outputstring;
    if (hintnumber > 15){
        outputstring="Your probe reveals nothing...";
    }
    else{
    
    if (hintnumber <= 8 ){
        int vectorsize=input.size();
        outputstring+="You find the length of the combo to be "+to_string(vectorsize)+" numbers in length. \n";
    }
    if (hintnumber >=3 && hintnumber <= 6)
    {
        int vectorsize=input.size() - 1;
        int firstindex = rand() % vectorsize;
        int secondindex = rand() % vectorsize;
        float ratiovalue = getRatio(input,firstindex,secondindex);
        outputstring += "Probe reveals value "+to_string(secondindex+1)+" / value "+to_string(firstindex+1)+" is "+to_string(ratiovalue)+"\n";
    }
    if (hintnumber >6 && hintnumber <=9)
    {
        int vectorsize=input.size() - 1;
        int firstindex = rand() % vectorsize;
        int secondindex = rand() % vectorsize;
        int indexsum = getSum(input,firstindex,secondindex);
        outputstring += "Probe reveals value "+to_string(secondindex+1)+" + value "+to_string(firstindex+1)+" is "+ to_string(indexsum)+"\n";

    }
    if (hintnumber >=9 && hintnumber <=12)
    {
         int vectorsize=input.size() - 1;
        int firstindex = rand() % vectorsize;
        int secondindex = rand() % vectorsize;
        int indexproduct = getProduct(input,firstindex,secondindex);
        outputstring += "Probe reveals value "+to_string(secondindex+1)+" * value "+to_string(firstindex+1)+" is "+ to_string(indexproduct)+"\n";

    }
    if (hintnumber >=12 && hintnumber <=15){
        int vectorsize=input.size() - 1;
        int firstindex = rand() % vectorsize;
        int secondindex = rand() % vectorsize;
        int indexsum = getDifference(input,firstindex,secondindex);
        outputstring += "Probe reveals value "+to_string(secondindex+1)+" - value "+to_string(firstindex+1)+" is "+ to_string(indexsum)+"\n";

    }
    if (hintnumber >=0 && hintnumber <=3)
    {
       int indexsum=getTotalSum(input);
       outputstring += "Probe reveals total sum of all values to be "+to_string(indexsum);
    }
    
    


    }
    return outputstring;

}



bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

//This function needs to compare the two vectors, which could be of any size.
//Pick the smaller of the two. Check the indexes for matching values. Return number
//of matching values. If same size, pick either and iterate.
int compareVectors(std::vector<int> const &realValues,std::vector<string> const &agentInput)
{
int counter;
if (realValues.size() > agentInput.size()){

for (size_t i =0; i< agentInput.size(); ++i){
    if (is_number(agentInput[i])){
        if (stoi(agentInput[i])==realValues[i]){
            ++counter;
        }
    }
}

}
else{
    for (size_t i =0; i< realValues.size(); ++i){
    if (is_number(agentInput[i])){
        if (stoi(agentInput[i])==realValues[i]){
            ++counter;
        }
    }


}
}
return counter; //logically to keep the puzzle game puzzling, we only want to reveal
//the number of correct slots filled, but not how many slots there are, nor what the numbers are.
//This makes the hints valuable in this way.
}

void helpPrompt(){

    cout << "You can enter 'help' for this screen. \n";
    cout << "You can enter 'probe' to attempt to get hints into the combination. \n";
    cout << "You can type 'status' to check how many probes and total tries you have remaining. \n";
    cout << "You can type 'review' to review the information you've gotten from your probes. \n";
    cout << "You can type 'quit' to end the game.\n";
    cout << "All other inputs will be considered valid tries and will count against you! \n";
    cout << "Based on difficulty, every few attempts will earn you another probe.\n";
}

int playGame(int NumTries, int NumHints, string AgentInput,int levelset,vector <int> ProblemSet,list<string> HintList){

bool notcompleted = true;
    int MatchingValues=0;
    int NewProbeCounter=0;
    while (notcompleted){
        if (NumTries > 0)
        {
        cout << "Please enter either a guess or a command: ";
        getline(std::cin,AgentInput);
        if (AgentInput=="help"){
            //do the help prompt
            helpPrompt();
        }
        else if (AgentInput=="probe"){
            //do the probe logic
            if (NumHints > 0){
                string ProbeHint=giveHint(ProblemSet,levelset);
                HintList.push_back (ProbeHint);
                cout << "Probe result: \n";
                cout << ProbeHint + "\n";
                --NumHints;
                cout << "Probes remaining: "+to_string(NumHints)+"\n";

            }
            else{
                cout << "You're out of probes! Review your hints with 'review'\n";
            }
        }
        else if (AgentInput=="quit"){
            notcompleted=false;
            return 3;
            //cout << "Thanks for playing. Goodbye!";
        }
        else if (AgentInput=="status"){
            //do the status logic
            cout << "You have: \n";
            cout << to_string(NumTries)+" tries remaining, \n";
            cout << to_string(NumHints)+" probes remaining, \n";
            cout << "Matched "+to_string(MatchingValues)+" values in the combo. \n";
        }
        else if (AgentInput=="review"){
            cout << "Here's the current hints you've gathered so far: \n";
            listPrint(HintList);
            //do the review logic
        }
        else{
            
            --NumTries;
            ++NewProbeCounter;
            if (((levelset+1)/NewProbeCounter)==1)
            {
                NewProbeCounter=0;
                ++NumHints;
            }
            //assume this is a valid try and evaluate
            istringstream buf(AgentInput);
            istream_iterator<string> beg(buf), end;
            vector<string> InputTokens(beg,end);
            MatchingValues=compareVectors(ProblemSet,InputTokens);
            if (MatchingValues==ProblemSet.size()){
                notcompleted=false;
                return 1;
                //cout << "Correct! You have found the combination! ";
            }
            else{
                cout << "You have "+to_string(MatchingValues)+" values correct, but not the full combination. \n";
            }

        
    }
        }
        else{
            notcompleted=false;
            return 2;
           // cout << "You have run out of tries! GAME OVER! \n";
           // cout << "The correct combination was: \n";
            //vectorPrint(ProblemSet);
        }
    }

}

bool PromptAgain(){
    string PlayAgain;
    cout << "\n Would you like to play again? (Y/N) ";
    getline(std::cin,PlayAgain);
    std::transform(PlayAgain.begin(), PlayAgain.end(),PlayAgain.begin(), ::toupper);
    while (PlayAgain != "Y" && PlayAgain != "N"){
        cout << "\n Please enter 'Y' or 'N' :";
        getline(std::cin,PlayAgain);
    }
    if (PlayAgain=="Y"){
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{   
    //we do playgame function in a loop. If it returns 1, player wins, play 
    //winning message.
    //If it returns 2, play losing message. If either 1 or 2 are presented, provide a 
    //replay prompt. 
    // If 3 is returned, that's a quit, provide a goodbye with no replay prompt.

    bool replay = true;
    while (replay){
        //this way you get the entire game experience on replay. Easy!
        string SetLevel;
    cout << "Please enter a level, 0, 1 or 2 (easy, Medium, or HARD): ";
    getline(std::cin,SetLevel);
    bool NotProperLevel=true;
    while (NotProperLevel)
    {
        
        if (is_number(SetLevel)){
            int check=stoi(SetLevel);
            if(check>=0 && check<=2){
                NotProperLevel=false;

            }
            else{
                cout << "Please enter a level, 0, 1 or 2: ";
                getline(std::cin,SetLevel);
            }
        }
        else{
                cout << "Please enter a level, 0, 1 or 2: ";
                getline(std::cin,SetLevel);
        }
    }
    int levelset=stoi(SetLevel); //let's have that difficulty setting, well, settable!
    string AgentType=GetAgentName(levelset); //bad name! UE uses UpperCamelCase naming convention
    string AgentInput;
    list<string> HintList;
    int NumHints=((levelset+1)*3)-levelset;
    int NumTries=10-(2*levelset);
    
    //const makes sure variables cannot be changed
    vector <int> ProblemSet = GetProblemSet(levelset);

    std::cout << "Welcome, "+AgentType+"\n";
    //you can also use std::cout << std::endl;
    std::cout << "Enter in the correct combo to break into the server room. \n";
    std::cout << "To continue, enter the correct code... \n";
    std::cout << "This code will be a series of numbers separated by a space (' '). \n";
    std::cout << "For example: '8 12 5 124 3'  \n";
    cout << "You can also type 'probe' to scan for a hint, and 'help' for commands. \n";
        int GameStatus=playGame(NumTries,NumHints,AgentInput,levelset,ProblemSet,HintList);
        if (GameStatus==1){
            cout << "You're In! You guessed the right combination.";
            replay=PromptAgain();
        }
        else if (GameStatus==2){
             cout << "You have run out of tries! GAME OVER! \n";
             cout << "The correct combination was: \n";
            vectorPrint(ProblemSet);
            replay=PromptAgain();
        }
        else if (GameStatus==3){
            
            replay=false;
        }
    }
    cout << "Thanks for playing. Goodbye!";

    


    return 0;
}