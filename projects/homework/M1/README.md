## 心路历程

一开始想写一个带自动排序的多叉树。

### OrderedList

自动排序用multiset，然后封装了一下multiset，发现除了套了一层皮之外，一点变化都没有。删掉OrderedList。准备在Tree中 吧multiset用using改名，好看一点。然后发现这样非常不灵活，时间复杂度也一样，根本不如直接把所有children装在一起，然后需要排序的时候sort一下。所以该模块全删了

### Tree

一开始的打算是，真的写一个树，然后用pid和一个map标记树节点指针的位置，实现O(1)的插入。然后在思考用不用unique pointer的时候，发现树的结构好像根本没有用，只需要一个map就能实现。map<int,vector>，排序的时候就遍历一下map，挨个sort就行，迭代的时候就从0开始，递归。

map用来记录Tree结构的信息，再用一个map用来记录name(或者干脆做一个结构体，写在一起也行)

