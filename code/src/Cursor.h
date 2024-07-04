//
// Created by tillw on 04/07/2024.
//

#ifndef CURSOR_H
#define CURSOR_H
#include "raylib.h"

enum CursorState
{
	Uninitialized, Locked, Unlocked
};

inline CursorState cursorState = Uninitialized;

void updateCursor()
{
	if (!IsCursorHidden() && cursorState == Locked)
	{
		DisableCursor();
	}

	if (cursorState == Uninitialized)
	{
		DisableCursor();
		cursorState = Locked;
	}

	if (IsKeyPressed(KEY_F1))
	{
		if (cursorState == Locked)
		{
			EnableCursor();
			cursorState = Unlocked;
		}
		else
		{
			DisableCursor();
			cursorState = Locked;
		}
	}
}

#endif //CURSOR_H
