#pragma once

#include <vector>
#include <memory>

namespace Editor::Datastructure
{
	/**
	 * Container that allows a class to manage its content
	 * @tparam C: Type of content inside container
	 */
	template<class C>
	class Container
	{
	protected:
		/**
		 * List of contents
		 */
		std::vector<std::unique_ptr<C>> m_contents;

	public:
		/**
		 * Add content
		 * @tparam T: Specific content class
		 * @tparam args: Arguments of constructor for given content class
		 * @return Instance of content
		 */
		template<class T, class ...Args>
		T& Add(Args&&... args);

		/**
		 * Remove content
		 * @param content: Content to remove
		 * @warning Don't call it when content is drawing/used
		 */
		void Remove(const C& content);

		/**
		 * Remove all content
		 * @warning Don't call it content widget is drawing/used
		 */
		void RemoveAll();
	};

	template<class C>
	template<class T, class ...Args>
	inline T& Container<C>::Add(Args&& ...args)
	{
		static_assert(std::is_base_of<C, T>::value);
		std::unique_ptr<T> content = std::make_unique<T>(std::forward<Args>(args)...);
		T& output = *content;
		m_contents.emplace_back(std::move(content));
		return output;
	}

	template<class C>
	inline void Container<C>::Remove(const C& content)
	{
		auto found = std::find_if(m_contents.begin(), m_contents.end(),
			[&content](std::unique_ptr<C>& c)
			{
				return c.get() == &content;
			});

		if (found != m_contents.end())
		{
			m_contents.erase(found);
		}
	}

	template<class C>
	inline void Container<C>::RemoveAll()
	{
		m_contents.clear();
	}
}
