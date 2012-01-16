package nodeSim;
/**
 * 
 */

/**
 * Represents a node in the list
 */
class Node {
	protected int sentRequests;
	protected int receivedRequests;
	protected int optimalPosition;
	protected int currentPosition;
	
	/**
	 * Constructor.
	 * @param optimalPosition this node's optimal position in the list
	 * @param currentPosition this node's initial position in the list
	 */
	public Node(int optimalPosition, int currentPosition) {
		this.optimalPosition = optimalPosition;
		this.currentPosition = currentPosition;
	}
			
			
	/**
	 * @return the sentRequests
	 */
	public int getSentRequests() {
		return sentRequests;
	}
	/**
	 * 
	 */
	public void incSentRequests() {
		this.sentRequests++;
	}
	/**
	 * @return the receivedRequests
	 */
	public int getReceivedRequests() {
		return receivedRequests;
	}
	/**
	 * 
	 */
	public void incReceivedRequests() {
		this.receivedRequests++;
	}
	/**
	 * @return the currentPosition
	 */
	public int getCurrentPosition() {
		return currentPosition;
	}
	/**
	 * @param currentPosition the currentPosition to set
	 */
	public void setCurrentPosition(int currentPosition) {
		this.currentPosition = currentPosition;
	}
	
	/**
	 * Increase the current position value by one
	 */
	public void incCurrentPosition()  {
		this.currentPosition++;
	}
	
	/**
	 * Decrease the current position value by one
	 */
	public void decCurrentPosition()  {
		this.currentPosition--;
	}
	
	/**
	 * @return the optimalPosition
	 */
	public int getOptimalPosition() {
		return optimalPosition;
	}
}
