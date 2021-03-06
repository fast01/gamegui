
#pragma once

#include "panel.h"

namespace gui
{
	class  BaseList : public Panel
	{
	public:
		typedef BaseList Self;
		BaseList(System& sys, const std::string& name = std::string());
		virtual ~BaseList();

		static const char* GetType() { return "BaseList"; }
		virtual const char* getType() const { return Self::GetType(); }

		virtual void Clear();

		virtual void render(const Rect& finalRect, const Rect& finalClip);
		virtual void init(xml::node& node);
		virtual bool onLoad();

		void setColumns(unsigned int col) { m_columns = col > 0 ? col : 1; }
		unsigned int getColumns() const { return m_columns; }
		void setColumnWidth(float w) { m_colWidth = w; }
		float getColumnWidth() const { return m_colWidth; }

	protected:
		virtual void layoutItems();
		virtual void onChildAdd(window_ptr& node);
		virtual void onChildRemove(window_ptr& node);
		virtual bool onResumeLayout();

	protected:
		bool m_drawpanel;
		unsigned int m_columns;
		float m_colWidth;
		Rect m_borders;
	};

	class Label;
	class Font;
	typedef std::shared_ptr<Font> FontPtr;

	class  ListBox : public BaseList
	{
	public:
		typedef ListBox Self;
		ListBox(System& sys, const std::string& name = std::string());
		virtual ~ListBox(void);

		static const char* GetType() { return "ListBox"; }
		virtual const char* getType() const { return Self::GetType(); }

		void AddItem(const std::string& name);

		virtual bool onMouseMove(void);
		virtual bool onMouseButton(EventArgs::MouseButtons btn, EventArgs::ButtonState state);
		virtual bool onMouseEnter(void);
		virtual bool onMouseLeave(void);

		virtual void render(const Rect& finalRect, const Rect& finalClip);
		virtual void init(xml::node& node);

		Label* GetSelectedItem() const;

	protected:
		virtual void onChildRemove(window_ptr& node);

	protected:
		FontPtr m_font;
		ImagesetPtr m_imgset;
		const Image* m_selection;
		window_ptr m_selectedItem;
	};

	class  CategorizedList : public BaseList
	{
	public:
		struct Category
		{
			Category(CategorizedList& p, const std::string& n) 
				: parent(p)
				, collapsed(true)
				, name(n)
			{}

			bool collapsed;
			std::string name;
			Rect area;
			Rect clickable;
			std::vector<window_ptr> children;
			CategorizedList& parent;

			void Rename(const std::string& n);
			unsigned int Add(base_window* wnd);
			void Remove(unsigned int idx);
			base_window*	GetWndByIndex(unsigned int idx);
			unsigned int GetChildrenCount() const;
			void Collapse() { Fold(true); }
			void Expand() { Fold(false); }
			void Fold(bool status);
			bool IsCollapsed() const { return collapsed; }

			float CalcHeight();
			void layoutChildren();

			struct VisibleTask
			{
				bool visible;
				VisibleTask(bool v) : visible(v) {}
				void operator () (window_ptr w) { if(w) w->setVisible(visible); }
			};
			struct HeightCalcTask
			{
				float& height;
				HeightCalcTask(float& h) : height(h) {}
				void operator () (window_ptr w) { if(w) height += w->getArea().getHeight(); }
				HeightCalcTask& operator=(const HeightCalcTask& rhs) {rhs;}
			};
		private:
			Category& operator=(const Category& rhs) {rhs;}
		};
		typedef std::shared_ptr<Category> CategoryPtr;

		typedef CategorizedList Self;
		CategorizedList(System& sys, const std::string& name = std::string());
		virtual ~CategorizedList(void);

		static const char* GetType() { return "CategorizedList"; }
		virtual const char* getType() const { return Self::GetType(); }

		unsigned int AddCategory(const std::string& name);
		Category* GetCategoryByIndex(unsigned int idx);
		//Category* GetCategoryByName(const std::string& name);
		unsigned int GetCategoryCount() const;
		void RemoveCategory(unsigned int idx);
		virtual void Clear();
		FontPtr GetFont() const { return m_font; }

		virtual bool onMouseButton(EventArgs::MouseButtons btn, EventArgs::ButtonState state);

		virtual void render(const Rect& finalRect, const Rect& finalClip);
		virtual void init(xml::node& node);

	protected:
		virtual void layoutItems();
		virtual void onChildAdd(window_ptr& node);
		virtual void onChildRemove(window_ptr& node);

	protected:
		FontPtr m_font;
		float m_indent;
		ImagesetPtr m_imgset;
		const Image* m_folded;
		const Image* m_unfolded;

		typedef std::vector<CategoryPtr> Categories;
		typedef Categories::iterator CategoriesIter;
		Categories m_categories;
	};

}
