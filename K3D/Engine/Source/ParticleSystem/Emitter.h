#pragma once
namespace K3D {

	class ParticleItem;



	
	class Emitter
	{
	public:

	private:
		
		std::function<void()> _customUpdater;

		std::vector < std::unique_ptr<ParticleItem> >_items;


	public:
		
		Emitter();

		~Emitter();

		void UpdateItems();

		void SetCustomUpdater(std::function<void(void)> function);
		
		template <class T>
		Emitter& AddItems()
		{
			_items.push_back(std::make_unique<T>());
			return *this;
		};

	};

}