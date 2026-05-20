#include<iostream>
#include<string>
using namespace std;

#ifndef Field_H
#define Field_H
class Field {

public:
	Field(int**, int, int, int, int);
	Field(const char*, int, int, int, int);
	~Field();

	bool move(char);
	bool moveUp(void);
	bool moveDown(void);
	bool moveLeft(void);
	bool moveRight(void);

	int getCurrentPositionX(void) const;
	int getCurrentPositionY(void) const;
	int getVisionWidth(void) const;
	int getVisionHeight(void) const;
	string getMapName(void) const;
	int getMapSymbol(int, int);
	void setPosition(int, int);
	void setMapSymbol(int, int, int);
	void setVisionSize(int, int);
	void display(void) const;

private:
	int** map_data;
	int current_position_x;
	int current_position_y;
	string map_name;
	int vision_width;
	int vision_height;
    
    // Internal helper for HW4 Maze loading
    void loadFromMazeFile(const char* filename);
};
#endif
