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

	// ����ʹ���ĸ����ʣ��ɾ��������ݺ궨��ȥѡ��
	virtual void set_texture(SDL_Texture **textures) = 0;

	SDL_Texture* get_texture();

	Vec2f speed;
	//POINT get_pos();
	// ������Ϣ��set��get
	void set_speed_x(float s);
	void set_speed_y(float s);
	void set_pos(float x, float y);
	void set_pos(Vec2f p);
	Vec2f get_pos();
	Vec4f get_geometry();
	SDL_Rect get_geometry_sdl();
	void set_geometry(float x, float y, float w, float h);
	void set_geometry(Vec4f rect);
	void set_width(float w);

	virtual void update(float delta_time=1.0f);

	// ��ȡͼ��ı߽�����
	int left();
	int right();
	int top();
	int bottom();
	Vec2f center();
	// ��ȡͼ����е�����
	int get_v_mid();
	int get_h_mid();
	SDL_Texture **textures;
protected:
	SDL_Texture *texture;
	Vec4f geometry;

};

