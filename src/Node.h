//

class Node {
	public:
		virtual dump() = 0;

	private:
		std::list<Node> children;
}
