#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/Symbol.h>
#include <gui/Button.h>
#include <gui/ComboBox.h>

class ToolBar : public gui::ToolBar {
protected:
	gui::Image _load;
	gui::Image _bfs;
	gui::Image _dfs;

public:
	ToolBar()
		: gui::ToolBar("mainTB", 1)
		, _bfs(":bfs")
		, _dfs(":dfs")
		, _load(":load")
	{
		addItem(tr("Load"), &_load, tr("Ucitaj maze"), 0, 0, 0, 0);
		addSpaceItem();
		addSpaceItem();
		addSpaceItem();

		addItem(tr("BFS"), &_bfs, tr("BFS"), 1, 0, 0, 1);
		addItem(tr("DFS"), &_dfs, tr("DFS"), 2, 0, 0, 2);

		//addItem()

	}

};