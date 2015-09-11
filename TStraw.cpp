#include <TEllipse.h>
#include <TText.h>
#include <sstream>
#include <string>


class TStraw{
	float x_pos;
	float y_pos;
	float radius;

	unsigned int id;

	long int value;

	TEllipse * shape;
	TText * lable;
public:
	static unsigned int number;
	TStraw(float x, float y, float r):x_pos(x), y_pos(y),radius(r){
		std::ostringstream s;
		s << number;
		shape = new TEllipse(x_pos, y_pos, radius, radius);
		lable = new TText(x_pos, y_pos, s.str().c_str());
		id = number;
		number++;
		lable = new TText(x_pos-(radius/2),y_pos-(radius/2),(s.str()).c_str() );		
		lable -> SetTextSize(radius);
	}
	~TStraw(){
		number--;
	}
	void Draw();
	void Mark(int);	
	void Mark();	
	void SetX1(double);	
	void SetY1(double);	
	void SetId(unsigned int);
	unsigned int GetId();

};
unsigned int  TStraw::number = 0;


void TStraw::Mark(int color ){
	shape->SetFillColor(color);
}
void TStraw::Mark(){
	shape->SetFillColor(9);
}

void TStraw::Draw(){
	shape->Draw();
	lable->Draw();	
}

void TStraw::SetX1(double new_x){
	shape->SetX1(new_x);
	lable->SetX(new_x-(radius/2));
	x_pos = new_x;
}

void TStraw::SetY1(double new_y){
	shape->SetY1(new_y);
	lable->SetY(new_y-(radius/2));
	y_pos = new_y;

}

void TStraw::SetId(unsigned int i){
	id = i;
}

unsigned int TStraw::GetId(){
	return id;
}
