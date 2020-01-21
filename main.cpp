#include<iostream>
#include<vector>
#include<string>
#include<climits>

using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;
void printString(string S);
void printChar(vector<int> x);
void printInt(vector<int> x);
void print_t (vector<bool> x);
void NewNamesAndOccurances(vector<int> & T, vector<int> & Occurances,
                            int StringSize, string S, int & charsUnique);
void SAIS (vector<int> Occurances, vector<int> & SA, vector<int> T, int iter);
string returnString (int index, vector<bool> t, string S);
bool compareStrings(int indexOne, int indexTwo, vector<bool> t, vector<int> T);

//------------MAIN---------------
int main () {
	string input;
	string S;
  while (getline(cin, input)) {
    S = S + input;
  }
  int StringSize = S.size() + 1;
  vector <int> T(StringSize, 0); //or T in her notes
  vector <int> Occurances;
  vector<int> SA (StringSize, -1);
  int charsUnique = 0;
  NewNamesAndOccurances(T, Occurances, StringSize, S, charsUnique);
  SAIS (Occurances, SA, T, 1);
  string answer;
  for (int i = 0; i < SA.size(); i++) {
    if (SA[i] != 0)
    answer.push_back(S[SA[i]-1]);
  }
  cout << answer << endl;
return 0;
}
//------------MAIN END---------------


void SAIS (vector<int> Occurances, vector<int> & SA, vector<int> T, int iter) {
  // cout << "Iteration " << iter << endl;
  // cout << "T at iteration "<< iter << endl;
  // printInt(T);

  vector<int> SA1 (T.size(), -1);
  if (Occurances.size() >= T.size()) {
    for (int i = 0; i < (int) T.size(); i++) {
      SA1[T[i]] = i;
    }
  } else {
  vector<int> front (Occurances.size(), -1);
  vector<int> back (Occurances.size(), -1);
  vector<int> backOriginal (Occurances.size(), -1);
  vector<int> frontOriginal (Occurances.size(), -1);
  vector<int> L (T.size(), 0);
  vector<bool> t (T.size());
  front[0] = 0; back[0] = 0;
  for (int i = 1; i < (int) Occurances.size(); i++) {
    front[i] = 1 + back[i - 1];
    back[i] = front[i] + Occurances[i] - 1;
  }
  //******************         Added for step 4 to help with output
  SA.clear(); SA.resize(T.size(), -1);
  backOriginal = back;
  frontOriginal = front;
  //Make t and place LMS' in SA and place pointers
  //L is false, S is true
  t[t.size() - 1] = true;
  SA[0] = t.size() - 1;
  for (int i = T.size() - 2; i >= 0; i--) {
    if (T[i] < T[i + 1]) {
      t[i] = true;
    }
    else if (T[i] > T[i + 1]){
      t[i] = false;
    }
    else
      t[i] = t[i + 1];
    if (i > 0) {
      //if its an S and the value to the left is in the original string is L then.
      if (t[i] == true && T[i - 1] > T[i]) {
        SA[back[T[i]]] = i;
        back[T[i]]--;
      }
    }
  }
  int index = 0;
  //**** NOW LETS GO LEFT TO RIGHT and put in L-TYPES
  for (int i = 0; i < (int) T.size(); i++) {
    index = SA[i] - 1;
    if (index >= 0) {
      if (t[index] == false) { //if L size
        SA[front[T[index]]] = index;
        front[T[index]]++;
      }
    }
  }
  //**** NOW LETS GO RIGHT TO LEFT
  back = backOriginal; //reset back vector here.
  L[0] = 1;
  for (int i = T.size() - 1; i > 0; i--) {
    index = SA[i] - 1;
    if (index >= 0) {
      if (t[index] == true) {
        SA[back[T[index]]] = index;
        back[T[index]]--;
	      if (index >= 1) {
          if (t[index - 1] == false) {
            L[back[T[SA[i]-1]] + 1] = 1;
          }
	      }
      }
    }
  }
  vector<int> N (T.size(), -1);
  int Ncounter = 0;
  N[N.size() - 1] = 0;
  int indexOne = SA[0];
  int indexTwo = 0;
  int OccurCounter = 1;
  for (int i = 1; i < (int) N.size(); i++) {
    if (L[i] == 1) {
      indexTwo = SA[i];
      bool result = compareStrings(indexOne, indexTwo, t, T);
      if (result == true) {
        N[SA[i]] = Ncounter;
      } else {
	        OccurCounter++;
          Ncounter++;
          N[SA[i]] = Ncounter;
      }
      indexOne = indexTwo;
    }
  }
  vector<int> T1;
  for (int i = 0; i < (int) N.size(); i++) {
    if (N[i] != -1) {
      T1.push_back(N[i]);
    }
  }
  //  cout << "type Array" << endl;
  // for(int i = 0; i < (int) T.size(); i++){
  //   cout << t[i] << " ";}
  //   cout << '\n';
  // cout << "LMS prefixes are sorted " << iter << endl;
  // for(int i = 0; i < (int) T.size(); i++){
  //   cout << SA[i] << " ";}
  //   cout << '\n';
  // cout << "T1 at iteration" << iter << endl;
  // for(int i = 0; i < (int) T1.size(); i++){
  //   cout << T1[i] << " ";}
  //   cout << '\n';
  // cout << "Iteration " << iter << ". Recursive call on T1."<< "\n********************************************" << endl;
  iter++;
  Occurances.clear();
  Occurances.resize(OccurCounter, 0);
  //printInt(Occurances);
  for (int i = 0; i < (int) T1.size(); i++) {
    Occurances[T1[i]]++;
  }
  SAIS (Occurances, SA1, T1, iter);

  iter--;

  // cout << "\n********************************************"<< "\nAfter recursive call inside iteration " << iter << endl;cout << "SA1 at iteration " << iter << " (after recursive call) is:" << endl;
  // for(int i = 0; i < (int)SA1.size(); i++)
  //   cout << SA1[i] << " " ;
  //   cout << endl;
  // cout << " T " << endl; printInt(T);
  // cout << " T1 " << endl; printInt(T1);
  // cout << " t " << endl; print_t(t);
  back = backOriginal;
  // cout << "SA at the end of SAIS function just before the return at iteration "<< iter <<endl;
  // printInt(SA);
  // return;
  for (int i = 0; i < SA.size(); i++) {
    SA[i] = -1;
  }
  for (int i = 0; i < t.size(); i++){
    if (t[i] == true && T[i - 1] > T[i]) {
      SA[back[T[i]]] = i;
      back[T[i]]--;
    }
  }
  front = frontOriginal;
  //**** NOW LETS GO LEFT TO RIGHT and put in L-TYPES
  for (int i = 0; i < (int) T.size(); i++) {
    index = SA[i] - 1;
    if (index >= 0) {
      if (t[index] == false) { //if L size
        SA[front[T[index]]] = index;
        front[T[index]]++;
      }
    }
  }
  //**** NOW LETS GO RIGHT TO LEFT
  back = backOriginal; //reset back vector here.
  L[0] = 1;
  for (int i = T.size() - 1; i > 0; i--) {
    index = SA[i] - 1;
    if (index >= 0) {
      if (t[index] == true) {
        SA[back[T[index]]] = index;
        back[T[index]]--;
	      if (index >= 1) {
          if (t[index - 1] == false) {
            L[back[T[SA[i]-1]] + 1] = 1;
          }
	      }
      }
    }
  }
  // cout << "SA at the end of SAIS function just before the return at iteration "<< iter <<endl;
  // printInt(SA);
  return;

  }//else
}











void NewNamesAndOccurances(vector<int> & T, vector<int> & Occurances,
                            int StringSize, string S, int & charsUnique) {
  vector <int> count (256, 0);
  charsUnique++; //since we need to count $
  for (int i = 0; i < StringSize - 1; i++) {
    if (count[(int)S[i]] == 0) {
      charsUnique++;
    }
    count[(int)S[i]] = 1;
  }
  int intName = 1;
  for (int i = 0; i < 256; i++) {
    if (count[i] > 0)
      count[i] = intName++;
  }
  Occurances.resize(charsUnique);
  T[StringSize - 1] = 0;
  for (int i = 0; i < (int)T.size(); i++) {
    T[i] = count[S[i]];
    Occurances[T[i]]++;
  }
}

bool compareStrings(int indexOne, int indexTwo, vector<bool> t, vector<int> T) {
  //int one = indexOne, two = indexTwo;
  while (T[indexOne] == T[indexTwo]) {
    indexOne++; indexTwo++;
    if (T[indexOne] != T[indexTwo])
      return false;
    //both end at this lms index thus are Equal.
    if ((t[indexOne] == true && t[indexOne - 1] != true) && (t[indexTwo] == true && t[indexTwo - 1] != true)) {
     //cout << "LMS STRINGS THAT MATCH" << endl;
     //string test = returnString(one, t, S); cout << "index " << one << " " << test << endl;
     //test = returnString(two, t, S); cout << "indexTwo " << two << " " << test << endl;
     return true;
    }
  }
  return false;
}

void print_t (vector<bool> x) {
  for (int i = 0; i < (int) x.size(); i++) {
    if (x[i] == true)
     cout << "S" <<" ";
    else
     cout << "L" << " ";
  }
  cout << '\n';
}

void printChar(vector<int> x) {
	for (int i = 0; i < (int) x.size(); i++) {
		cout << char(x[i]) << " ";
	} cout << '\n';
}
void printInt(vector<int> x) {
	for (int i = 0; i < (int) x.size(); i++) {
		cout << x[i] << " ";
	} cout << '\n';
}

void printString(string S) {
}

string returnString (int index, vector<bool> t, string S) {
  string answer;
  S = S + '$';
  int size = S.size();
  answer.push_back(S[index]);
  for (int i = index + 1; i < size; i++) {
    answer.push_back(S[i]);
    if (t[i] == true && t[i - 1] != true)
      i = size;
  }
  return answer;
}
