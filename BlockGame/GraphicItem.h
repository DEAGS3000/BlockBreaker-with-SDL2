#pragma once
#include <SDL.h>
#include <Windows.h>
#include "common_def.h"

class GraphicItem
{
public:
	GraphicItem(void);
	GraphicItem(SDL_Texture *t);
	~GraphicItem(void);

	// 具体使用哪个材质，由具体的类根据宏定义去选择
	virtual void set_texture(SDL_Texture **textures) = 0;

	SDL_Texture* get_texture();
	
	POINT speed;
	//POINT get_pos();
	// 几何信息的set和get
	void set_speed_x(int s);
	void set_speed_y(int s);
	void set_pos(int x, int y);
	void set_pos(POINT p);
	POINT get_pos();
	SDL_Rect get_geometry();
	void set_geometry(int x, int y, int w, int h);
	void set_geometry(SDL_Rect rect);
	void set_width(int w);

	virtual void update(float delta_time=1.0f);

	// 获取图像的边界坐标
	int get_left_edge();
	int get_right_edge();
	int get_top_edge();
	int get_bottom_edge();
	// 获取图像的中点坐标
	int get_v_mid();
	int get_h_mid();
	SDL_Texture **textures;
protected:
	SDL_Texture *texture;
	SDL_Rect geometry;
	
};

