#pragma once


namespace UIProjectExplorer {

	/**
	* Displays the list of Scene resources inside of the engine.
	*
	* NOTE(Afiq):
	* Need to be expanded for double clicking to open Scene viewer.
	*/
	void ShowSceneList(GravityApp *Application);

	/**
	* Displays the list of Texture resoruces inside of the engine.
	*
	* NOTE(Afiq):
	* Need to be expanded for double clicking to open Texture viewer.
	*/
	void ShowTextureList(GravityApp *Application);

	/**
	* Displays the list of Shader resoruces inside of the engine.
	*
	* NOTE(Afiq):
	* Need to be expanded for double clicking to open Shader viewer.
	*/
	void ShowShaderList(GravityApp *Application);

	/**
	* Displays the list of material resoruces inside of the engine.
	*
	* NOTE(Afiq):
	* Need to be expanded for double clicking to open Material viewer.
	*/
	void ShowMaterialList(GravityApp *Application);

	/**
	* Displays the list of material resoruces inside of the engine.
	*
	* NOTE(Afiq):
	* Need to be expanded for double clicking to open Material viewer.
	*/
	void ShowSceneryList(GravityApp *Application);

	/**
	* TODO(Afiq):
	* Add right click context menus for every component.
	*/
}