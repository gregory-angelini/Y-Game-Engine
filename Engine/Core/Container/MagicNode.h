#ifndef MAGICNODE_H
#define MAGICNODE_H
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
class MagicContainer;
class MagicNode
{
	class AddressList;
	public: class Address
	{
		friend class AddressList;
		friend class MagicNode;
		private: Address* left;
		private: Address* right;
		protected: AddressList* container;
		protected: Address(void);
		protected: Address(const Address& _node);
		public: virtual ~Address(void);
		public: Address& operator = (const Address& _node);
		public: bool operator == (const Address& _node);
		protected: void Remove(void);
		public: virtual MagicContainer* GetContainer(void) = 0;
	};
	private: class AddressList
	{
		friend class Address;
		private: Address* head;
		private: Address* tail;
		private: int32 size;
		private: bool isDestroyed;
		public: AddressList(void);
		public: AddressList(const AddressList& _container);
		public: virtual ~AddressList(void);
		private: void DestroyList(Address* _node);
	    public: bool IsDestroyed(void);
		public: int32 GetSize(void)const;
		public: Address* GetHead(void)const;
		public: Address* GetTail(void)const;
		public: Address* Find(const MagicContainer* _key)const;
		public: void AddToTail(Address* _node);
		public: void Clear(void);
		public: void Remove(Address* _node);
	};
	template<typename N> friend class MagicBinaryTree;
	template<typename N> friend class MagicList;
	template<typename N> friend class MagicPointer;
	template<typename N> friend class MagicListEx;
	private: AddressList connection;
	public: MagicNode(void);
	public: MagicNode(const MagicNode& _node);
	public: virtual ~MagicNode(void);
	public: MagicNode& operator = (const MagicNode& _node);
	public: bool operator == (const MagicNode& _node);
	public: int32 GetAmountOfContainers(void)const;
	public: SimpleList<MagicContainer*> GetContainers(void)const;
	public: Address* GetAddress(MagicContainer* _container)const;
	public: SimpleList<Address*> GetConnections(void);
    public: void DestroyAllConnections(void);
};




#endif
