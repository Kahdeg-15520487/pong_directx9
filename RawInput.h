#pragma once
class RawInput
{
	short mouseState[5];

	USHORT MASKDOWN[5] = { RI_MOUSE_LEFT_BUTTON_DOWN,
	RI_MOUSE_RIGHT_BUTTON_DOWN,
	RI_MOUSE_MIDDLE_BUTTON_DOWN,
	RI_MOUSE_BUTTON_4_DOWN,
	RI_MOUSE_BUTTON_5_DOWN };

	USHORT MASKUP[5] = { RI_MOUSE_LEFT_BUTTON_UP,
	RI_MOUSE_RIGHT_BUTTON_UP,
	RI_MOUSE_MIDDLE_BUTTON_UP,
	RI_MOUSE_BUTTON_4_UP,
	RI_MOUSE_BUTTON_5_UP };

public:
	RawInput() {}
	~RawInput() {}

	int mouseX;
	int mouseY;
	USHORT keyState[256];
	short mouseScroll;
	long mxr, myr;

	void Refresh() {
		mxr = 0;
		myr = 0;
	}

	void ProcessMouseMessage(const RAWMOUSE *rmouse)
	{
		// Store Mouse Button States
		for (int i = 0; i < 5; i++)
		{
			if (MASKDOWN[i] & rmouse->usButtonFlags)
				mouseState[i] = 1;
			else if (MASKUP[i] & rmouse->usButtonFlags)
				mouseState[i] = 0;
		}
		// Check Mouse Position Relative Motion
		if (MOUSE_MOVE_RELATIVE == rmouse->usFlags)
		{
			mxr = rmouse->lLastX;
			myr = rmouse->lLastY;
		}

		// Check Mouse Wheel Relative Motion
		mouseScroll = (RI_MOUSE_WHEEL & rmouse->usButtonFlags) ? (short)(rmouse->usButtonData) : 0;
	}
	void ProcessKeyboardMessage(const RAWKEYBOARD *rkey)
	{
		UINT Event;
		Event = rkey->Message;
		if (Event == WM_KEYUP)
		{
			keyState[rkey->VKey] = 0;
		}
		else if (Event == WM_KEYDOWN) {
			keyState[rkey->VKey] = 1;
		}
	}

	// current down
	bool IsKeyDown(USHORT key) {
		return keyState[key];
	}

	// current up
	bool IsKeyUp(USHORT key) {
		return !keyState[key];
	}
};