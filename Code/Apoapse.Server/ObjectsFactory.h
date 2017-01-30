#pragma once
#include <unordered_map>
#include <memory>

template <typename T_INDEX, typename T_BASE_CLASS>
class ObjectsFactory
{
	struct IObjectCreator
	{
		virtual std::unique_ptr<T_BASE_CLASS> Create() = 0;
	};

	template <typename T_CLASS>
	class Creator : public IObjectCreator
	{
	public:
		std::unique_ptr<T_BASE_CLASS> Create()
		{
			return std::make_unique<T_CLASS>();
		}
	};

	std::unordered_map<T_INDEX, std::unique_ptr<IObjectCreator>> m_objectFactories;

public:
	bool IsObjectExist(const T_INDEX& index) const
	{
		return (m_objectFactories.count(index) > 0);
	}

	std::unique_ptr<T_BASE_CLASS> CreateObject(const T_INDEX& index)
	{
		ASSERT_MSG(IsObjectExist(index), "The requested object does not exist");

		return m_objectFactories[index]->Create();
	}

	template <typename T_CLASS>
	void RegisterObject(const T_INDEX& index)
	{
		ASSERT_MSG(!IsObjectExist(index), "This object is already registered");

		m_objectFactories[index] = std::make_unique<Creator<T_CLASS>>();
	}
};

template <typename T_BASE_CLASS, typename T_CLASS, typename T_INDEX>
class ObjectAutoRegister
{
public:
	ObjectAutoRegister(ObjectsFactory<T_INDEX, T_BASE_CLASS>& objectsFactory, const T_INDEX& index)
	{
		objectsFactory.RegisterObject<T_CLASS>(index);
	}
};
#define OBJECTS_FACTORY_REGISTER(_factoryInstance, _baseClass, _class, _indexType, _index)	static ObjectAutoRegister<_baseClass, _class, _indexType> objRegistration(_factoryInstance, _index);