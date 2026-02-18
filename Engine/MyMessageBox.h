#pragma once
#include <string>

#include "Graphics.h"
#include "Mouse.h"
#include "Colors.h"
#include "Font.h"
#include "Button.h"

class MyMessageBox
{
public:
	enum class Buttons { YesNo, Ok };
	enum class ValueButton { Yes, No, Ok, None };

public:
	MyMessageBox(const Font* font)
		:
		font(font),
		buttonYes(font, "yes", Graphics::GetScreenCenter() + Vei2{ 0, Graphics::ScreenHeight / 8 }),
		buttonNo(font, "no", Graphics::GetScreenCenter() + Vei2{ 0, Graphics::ScreenHeight / 8 }),
		buttonOk(font, "ok", Graphics::GetScreenCenter() + Vei2{ 0, Graphics::ScreenHeight / 8 })
	{
		buttonYes.SetPositionCenter(true);
		buttonNo.SetPositionCenter(true);
		buttonOk.SetPositionCenter(true);
		buttonNo.SetSizeWidthContentBox(font->GetWidthChar() * 3);
		buttonOk.SetSizeWidthContentBox(font->GetWidthChar() * 3);
		UpdateLayout();
	}
	void Draw(Graphics& gfx) const
	{
		// Dark
		gfx.DrawDisabled(Graphics::GetScreenRect());

		// Background
		gfx.DrawRect(rect, Colors::Gray);

		// Text
		Vei2 posText;
		if (font->NumberOfLines(messageText) == 1) {
			posText = Vei2{ Graphics::GetScreenCenter().x, rect.top + rect.GetHeight() / 4 } - Vei2{ int(messageText.size()) * font->GetWidthChar() / 2, 0 };
		}
		else {
			posText = Vei2{ Graphics::GetScreenCenter().x, rect.top + rect.GetHeight() / 5 } - Vei2{ font->GetLongestLineSize(messageText) * font->GetWidthChar() / 2, 0 };
		}
		font->DrawText(messageText, posText, Colors::White, gfx);

		// Buttons
		switch (buttons)
		{
		case Buttons::YesNo:
			buttonYes.Draw(gfx);
			buttonNo.Draw(gfx);
			break;
		case Buttons::Ok:
			buttonOk.Draw(gfx);
			break;
		}
	}
	ValueButton ProcessMouse(const Mouse::Event& e)
	{
		switch (buttons)
		{
		case Buttons::YesNo:
			buttonYes.ProcessMouse(e);
			buttonNo.ProcessMouse(e);
			if (buttonYes.IsClicked()) return ValueButton::Yes;
			else if (buttonNo.IsClicked()) return ValueButton::No;
			break;

		case Buttons::Ok:
			buttonOk.ProcessMouse(e);
			if (buttonOk.IsClicked()) return ValueButton::Ok;
			break;
		}

		return ValueButton::None;
	}
	void SetButtons(Buttons buttons)
	{
		this->buttons = buttons;
		UpdateLayout();
	}
	void SetText(const std::string& text)
	{
		messageText = text;
	}

private:
	void UpdateLayout()
	{
		Vei2 buttonAreaCenter = { rect.GetCenter().x, rect.top + (rect.GetHeight() * 3 / 4) };
		int padding = 20;

		if (buttons == Buttons::YesNo)
		{
			int totalWidth = buttonYes.GetRect().GetWidth() + padding + buttonNo.GetRect().GetWidth();

			Vei2 posNo = buttonAreaCenter - Vei2{ totalWidth / 2 - buttonNo.GetRect().GetWidth() / 2, 0 };
			buttonNo.SetPos(posNo);

			Vei2 posYes = buttonAreaCenter + Vei2{ totalWidth / 2 - buttonYes.GetRect().GetWidth() / 2, 0 };
			buttonYes.SetPos(posYes);
		}
		else
		{
			buttonOk.SetPos(buttonAreaCenter);
		}
	}

private:
	static constexpr RectI rect = RectI{ Graphics::ScreenWidth / 5, Graphics::ScreenWidth / 5 * 4 , Graphics::ScreenHeight / 5, Graphics::ScreenHeight / 5 * 4 };
	const Font* font;
	std::string messageText = "Default";
	Buttons buttons = MyMessageBox::Buttons::YesNo;

	Button buttonYes;
	Button buttonNo;
	Button buttonOk;
};