#pragma once
#ifndef BASE_MODULE_HPP
#define BASE_MODULE_HPP

namespace Base {
	class Module
	{
	public:
		void initialize();

	protected:
		virtual void on_initialize() = 0;

	private:
		bool initialized_;
	};
}

#endif