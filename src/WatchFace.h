#pragma once

// Expanded
//#include "WatchyExpanded.h" // Here because every user has to include this.

class CWatchyExpanded;

class CWatchFace
{
	public:
		CWatchFace() = default;
		virtual void Draw(CWatchyExpanded& expanded) = 0; //override this method for different watch faces
};

class CWatchFaceHolderBase
{
	public:
		virtual CWatchFace* Get() = 0;
};

template <typename T>
class CWatchFaceHolder : public CWatchFaceHolderBase
{
	public:
		CWatchFace* Get() override
		{
			if (!m_pWatchFace)
				m_pWatchFace = new T;
			return m_pWatchFace;
		}

	private:
		CWatchFace* m_pWatchFace = nullptr;
};
