#pragma once

#include <sstream>
#include "Lithium/Events/Event.h"

namespace li
{
	class WindowEvent : public Event
	{
	public:
		WindowEvent(unsigned int id)
			: m_WindowID(id) {}

		inline unsigned int GetWindowID() { return m_WindowID; }

	private:
		unsigned int m_WindowID;
	};

	class WindowResizeEvent : public WindowEvent
	{
	public:
		WindowResizeEvent(unsigned int id, unsigned int width, unsigned int height)
			: WindowEvent(id), m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public WindowEvent
	{
	public:
		WindowCloseEvent(unsigned int id) 
		: WindowEvent(id) {}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowFocusGainedEvent : public WindowEvent
	{
	public:
		WindowFocusGainedEvent(unsigned int id)
		: WindowEvent(id) {}

		std::string ToString() const override
		{
			return "WindowFocusGainedEvent";
		}

		EVENT_CLASS_TYPE(WindowFocusGained)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowFocusLostEvent : public WindowEvent
	{
	public:
		WindowFocusLostEvent(unsigned int id)
		: WindowEvent(id) {}

		std::string ToString() const override
		{
			return "WindowFocusLostEvent";
		}

		EVENT_CLASS_TYPE(WindowFocusLost)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowMovedEvent : public WindowEvent
	{
	public:
		WindowMovedEvent(unsigned int id, int x, int y)
		: WindowEvent(id), m_X(x), m_Y(y) {}

		inline unsigned int GetX() const { return m_X; }
		inline unsigned int GetY() const { return m_Y; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: " << m_X << ", " << m_Y;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)

	private:
		int m_X, m_Y;
	};

	class WindowMinimizedEvent : public WindowEvent
	{
	public:
		WindowMinimizedEvent(unsigned int id)
			: WindowEvent(id) {}

		std::string ToString() const override
		{
			return "WindowMinimizedEvent";
		}

		EVENT_CLASS_TYPE(WindowMinimized)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowMaximizedEvent : public WindowEvent
	{
	public:
		WindowMaximizedEvent(unsigned int id)
			: WindowEvent(id) {}

		std::string ToString() const override
		{
			return "WindowMaximizedEvent";
		}

		EVENT_CLASS_TYPE(WindowMaximized)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowRestoredEvent : public WindowEvent
	{
	public:
		WindowRestoredEvent(unsigned int id)
			: WindowEvent(id) {}

		std::string ToString() const override
		{
			return "WindowRestoredEvent";
		}

		EVENT_CLASS_TYPE(WindowRestored)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};
}