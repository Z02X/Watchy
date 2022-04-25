#pragma once

template <typename TParent>
class CHolderBase
{
	public:
		virtual TParent* Get() = 0;
};

template <typename TType, typename TParent>
class CHolder : public CHolderBase<TParent>
{
	public:
		TParent* Get() override
		{
			if (!m_pHeld)
				m_pHeld = new TType;
			return m_pHeld;
		}

	private:
		TParent* m_pHeld = nullptr;
};
