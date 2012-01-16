/**
 * 
 */
package nodeSim.containers;


/**
 * Represents a request.
 */
public class Request {
	public Request(int source, int destination) {
		this.posS = source;
		this.posD = destination;
	}
	
	/** position of the source node (in the optimal arrangement) */
	protected int posS;
	
	/** position of the destination node (in the optimal arrangement) */
	protected int posD;
	
	/**
	 * @return the position of the source node (in the optimal arrangement)
	 */
	public int getPosS() {
		return posS;
	}
	/**
	 * @return the position of the destination node (in the optimal arrangement)
	 */
	public int getPosD() {
		return posD;
	}
}
