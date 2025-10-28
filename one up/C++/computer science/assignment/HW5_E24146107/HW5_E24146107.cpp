#include<iostream>
#include<vector>

#define debug(d) cout << #d << ": " << d << endl
#define fr(d) for(int i=0; i<d; i++)

using namespace std;

void DisksMoving(vector<int> Tower[],int Disks_Height, int TargetTower, int CurrentTower, int IdleTower) 
{	
	if(Disks_Height == int(Tower[CurrentTower].size())-1) //if the disk is at the top
	{
		
		int disks = Tower[CurrentTower][Tower[CurrentTower].size()-1]; //move it to the target tower
		Tower[TargetTower].push_back(disks);
		Tower[CurrentTower].pop_back();
		cout << "Disk " << disks << " moving from tower " << CurrentTower+1 << " to tower " << TargetTower+1 << endl;
	} else
	{
		int MovedDiskHeight = int(Tower[IdleTower].size());
		DisksMoving(Tower, Disks_Height+1,IdleTower, CurrentTower, TargetTower);  //move the disks above target disk to the idle tower
		DisksMoving(Tower, Disks_Height,TargetTower, CurrentTower, IdleTower);    //move the target disk to the target tower
		DisksMoving(Tower, MovedDiskHeight, TargetTower, IdleTower, CurrentTower);//move the disks that was above the target disk back to the target tower
		
	}
}

int main()
{
	//cout << "creating storage for disks." << endl;
	vector<int> Tower[3];




	cout << "Users input number of disks at the beginning." << endl;
	int NumberOfDisks;
	cin >> NumberOfDisks;
	fr(NumberOfDisks)
	{
		Tower[0].push_back(NumberOfDisks - i);
	}


	cout << "Show the initial settings on the screen." << endl;
	fr(3)
	{
		cout << "Tower " << i + 1 << ": ";
		for(int j=0; j<Tower[i].size();j++)
		{
			cout << Tower[i][j] << "";
		}
		cout << endl;
	}
	cout << endl;

	cout << "Show all the moves on the screen." << endl;
	DisksMoving(Tower,0,2,0,1);
	cout << endl;

	cout << "If all the disks are on Tower 3, print “WIN!”." << endl;
	fr(3) //shows that all the disks are on Tower 3
	{
		cout << "Tower " << i + 1 << ": ";
		for(int j=0; j<Tower[i].size();j++)
		{
			cout << Tower[i][j] << "";
		}
		cout << endl;
	}
	cout << "\"WIN!\"" << endl;
	cout << endl;

	cout << "Show your answers step by step." << endl;
	fr(3) //do the same thing as before
	{
		while(!Tower[i].empty())
		Tower[i].pop_back();
	}
	fr(NumberOfDisks)
	{
		Tower[0].push_back(NumberOfDisks - i);
	}
	DisksMoving(Tower,0,2,0,1);


	
	return 0;
}