#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>


using namespace std;

class Task {
public:
	int time; //ile trwa zadanie
	int ex; // ktore to zadanie
	Task() {
		time = 0;
		ex = 0;
	}
	Task(int t, int e) {
		time = t;
		ex = e;
	}

	bool operator==(const Task& rhs) const { return this->ex == rhs.ex; }
};
void delete_task(vector<vector<Task>> &macierz, int nr) {
	int size = macierz.size() - 1; //3
	macierz[nr].clear();
	for (int i = nr; i < size; i++)
		macierz[i].swap(macierz[i + 1]);
	macierz.resize(size);
}

int ifFound(vector<int> zbior, int szukany) {
	for (int i = 0; i < zbior.size(); i++)
		if (zbior[i] == szukany) return i;
	return -1;
}

Task min_kolumn(vector<vector<Task>> macierz, vector<int> zbior_nieuszeregowanych, int nrKolumny) {
	Task min;
	min.time = 99999;
	for (int i = 0; i < macierz.size(); i++) {
		if (ifFound(zbior_nieuszeregowanych, macierz[i][nrKolumny].ex) >= 0) {
			if (macierz[i][nrKolumny].time < min.time) min = macierz[i][nrKolumny];
		}
	}
	return min;
}

Task max_kolumn(vector<vector<Task>> macierz, vector<int> zbior_gotowych, int nrKolumny) {
	Task max;
	max.time = -1;
	for (int i = 0; i < macierz.size(); i++) {
		if (ifFound(zbior_gotowych, macierz[i][nrKolumny].ex) >= 0) {
			if (macierz[i][nrKolumny].time > max.time) max = macierz[i][nrKolumny];
		}
	}
	return max;
}

vector<int> shrage(vector<vector<Task>> macierz) {
	vector<vector<Task>> macierz_pom = macierz;
	vector<int> order2;
	int i = 0; vector<int> czesciowa_kolejnosc(0);
	Task j;
	vector<int> zbior_gotowych(0);
	vector<int> zbior_nieuszeregowanych(macierz.size());
	for (int i = 0; i < zbior_nieuszeregowanych.size(); i++) zbior_nieuszeregowanych[i] = i + 1;
	int t = min_kolumn(macierz, zbior_nieuszeregowanych, 0).time;
	while (zbior_gotowych.size() != 0 || zbior_nieuszeregowanych.size() != 0) {
		while (zbior_nieuszeregowanych.size() != 0 && min_kolumn(macierz, zbior_nieuszeregowanych, 0).time <= t) {
			j = min_kolumn(macierz, zbior_nieuszeregowanych, 0);
			zbior_gotowych.push_back(j.ex);
			int gowno = j.ex;
			vector<int>::iterator usunac;
			usunac = find(zbior_nieuszeregowanych.begin(), zbior_nieuszeregowanych.end(), j.ex);
			int index;
			zbior_nieuszeregowanych.erase(usunac);
			for (int i = 0; i < macierz_pom.size(); i++)
				if (macierz_pom[i][0].ex == j.ex)
					index = i;
			delete_task(macierz_pom, index);			
		}
		if (zbior_gotowych.size() == 0) {
			t = min_kolumn(macierz, zbior_nieuszeregowanych, 0).time;
		}
		else {
			j = max_kolumn(macierz, zbior_gotowych, 2);
			vector<int>::iterator usunac;
			usunac = find(zbior_gotowych.begin(), zbior_gotowych.end(), j.ex);
			czesciowa_kolejnosc.resize(czesciowa_kolejnosc.size() + 1);
			czesciowa_kolejnosc[i] = j.ex;
			int index = distance(zbior_gotowych.begin(), usunac);
			zbior_gotowych.erase(usunac);
			
			i++; t = t + macierz[j.ex - 1][1].time;
		}
	}
	order2 = czesciowa_kolejnosc;
	return order2;
}

vector<vector<Task>> read_data(int &number_of_ex, int &n_m) {
	ifstream data("in200.txt");
	data >> number_of_ex;
	data >> n_m;
	vector<vector<Task>> macierz(number_of_ex, vector<Task>(n_m));
	for (int j = 0; j < number_of_ex; j++) {
		for (int i = 0; i < n_m; i++) {
			data >> macierz[j][i].time;
			macierz[j][i].ex = j + 1;
		}
	}
	data.close();
	return macierz;
}
int cmax(vector<int> order, vector<vector<Task>> macierz, vector<int> Cm, int number_of_ex, int n_m) {
	int n_ex = number_of_ex + 1;
	vector<int> S(Cm.size()+1);
	S[0] = 0;
	S[1] = max(macierz[order[0] - 1][0].time, 0 + 0);
	Cm[0] = S[1] + macierz[order[0] - 1][1].time;
		for (int j = 2; j <= number_of_ex; j++) {
			S[j] = max(macierz[order[j-1] - 1][0].time, S[j - 1] + macierz[order[j - 2] - 1][1].time);
			Cm[j-1] = S[j] + macierz[order[j - 1]-1][1].time;
		}
		for(int i=0; i<Cm.size();i++) Cm[i] = Cm[i] + macierz[order[i]-1][2].time;
		vector<int>::iterator max = max_element(Cm.begin(), Cm.end());
		int ind = distance(Cm.begin(), max);
	return Cm[ind];
}

int main()
{
	chrono::time_point< std::chrono::system_clock> start = std::chrono::system_clock::now();
	int number_of_ex, n_m, min, min_ex, min_m;
	vector<vector<Task>> macierz2 = read_data(number_of_ex, n_m);
	vector<int> Cm(number_of_ex);

	vector<int> order = shrage(macierz2);
	int Cmax = cmax(order, macierz2, Cm, number_of_ex, n_m);
	cout << "Cmax = " << Cmax << endl;
	cout << "order = " << endl;
	for (int i = 0; i < order.size(); i++) cout << order[i] << " ";
	chrono::time_point< std::chrono::system_clock> end = std::chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	cout << "czas trwania: " << elapsed_seconds.count() << "s\n";
	return 0;
}
