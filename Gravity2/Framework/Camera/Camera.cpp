#include "stdafx.h"


GravityCamera::GravityCamera() :
	Projection(1.0f), 
	View(1.0f), 
	Viewport(0.0f), 
	Rotation(0.0f), 
	Position(0.0f), 
	Forward(), 
	Up(),
	Right(), 
	FieldOfView(0.0f), 
	Near(0.0f), 
	Far(0.0f), 
	MoveSpeed(0.0f), 
	Sensitivity(0.0f), 
	Zoom(0.0f), 
	Enable(true), 
	Dirty(false) {}