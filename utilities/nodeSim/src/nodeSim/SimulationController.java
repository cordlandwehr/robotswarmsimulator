package nodeSim;

import java.util.Queue;

import nodeSim.containers.Input;
import nodeSim.containers.Request;


/**
 * The superclass for the strategy implementations.
 * Each strategy is implemented as a subclass of this.
 */
public abstract class SimulationController {	
	/** The nodes in the _current_ arrangement */
	protected Node[] nodes;

	/** stores the current positions of the nodes (accessed by their optimal position) */
	protected int[] optimalToCurrent;
	
	/** The (yet) generated cost */
	protected int cost = 0;
	
	/**	The cost that would (yet) have been generated in an optimal arrangemen */
	protected int optimalCost = 0;

	/** number of already simulatedRounds */
	protected int elapsedRounds = 0;
	
	/** Queue containing the requests */
	protected Queue<Request> requests;

	/** store the last processed request - in case anyone wants to know */
	protected Request lastRequest;

	/** stores the total number of requests / rounds */
	protected int numberOfRounds;


	/**
	 * Constructor.
	 * @param input the input for this simulation
	 */
	public SimulationController(Input input) {
		// create and fill node array
		nodes = new Node[input.getNumberOfNodes()];
		optimalToCurrent = new int[input.getNumberOfNodes()];
		
		for (int i = 0; i < input.getNumberOfNodes();i++) {
			nodes[i] = new Node(input.getInitialArrangement()[i],i);
			optimalToCurrent[input.getInitialArrangement()[i]] = i;
		}
		
		//load request queue
		requests = input.getRequests();
		
		numberOfRounds = requests.size();
	}

	/**
	 * @return the elapsedRounds
	 */
	public int getElapsedRounds() {
		return elapsedRounds;
	}

	/**
	 * @return the cost
	 */
	public int getCost() {
		return cost;
	}
	
	/**
	 * @return the optimal cost
	 */
	public int getOptimalCost() {
		return optimalCost;
	}	

	/**
	 * @param posU position of the node u in the current arrangement
	 * @param posV position of the node v in the current arrangement
	 * @return the distance of the nodes u and v (in the optimal arrangement)
	 */
	protected int getDistanceInOptimal(int posU, int posV) {
		return Math.abs(nodes[posU].getOptimalPosition()-nodes[posV].getOptimalPosition());
	}
	
	/** 
	 * @param posU position of the node u in the optimal arrangement
	 * @param posV position of the node v in the optimal arrangement
	 * @return the distance of the nodes u and v (in the current arrangement)
	 */
	protected int getDistanceInCurrent(int posU, int posV) {
		return Math.abs(optimalToCurrent[posU]-optimalToCurrent[posV]);
	}

	/** @return the total number of nodes */
	public int getN() {
		return nodes.length;
	}
	
	/**
	 * @return the current potential (\sum_{(v,w)}{(d(v,w)-1)})
	 */
	public int getPotential() {
		int potential = 0;
		for(int i = 0; i < getN() - 1; i++) {
			potential += getDistanceInCurrent(i,i+1) - 1;
		}
		return potential;
	}
	
	/**
	 * @return the numberOfRounds
	 */
	public int getNumberOfRounds() {
		return numberOfRounds;
	}

	/**
	 * runs one simulation step
	 * @return 
	 */
	public boolean runOneStep() {
		if (requests.isEmpty())
			return false;
		
		Request request = requests.poll();
		processRequest(request);
		
		return !requests.isEmpty();
	}

	/**
	 * @return the last processed request
	 */
	public Request getLastRequest() {
		return lastRequest;
	}
	
	/**
	 * No copy or anything for performance reason - DON'T CHANGE ANYTHING!
	 * @return the current node array
	 */
	 Node[] getCurrentNodes() {
		return nodes;
	}

	/**
	 * Moves a node by one position (direction specified by dir)
	 * @param pos the position of the node to be moved
	 * @param dir the direction of the movement (-1 : left, 1 : right)
	 * @return the new position of the node
	 */
	protected int moveNode(int pos, int dir) {
		if (dir < 0)
			return moveNodeLeft(pos);
		else if (dir > 0 )
			return moveNodeRight(pos);
		
		return pos;
	}
	
	/** 
	 * Move the specified node one position to the left
	 * @param pos position of the node to be moved
	 * @return the new position of the node
	 */
	protected int moveNodeLeft(int pos) {
		if (pos == 0) {
			//trying to move node out of the left end of the list
			return pos;
		}

		Node rightNode = nodes[pos];
		Node leftNode = nodes[pos-1];
		
		//switch the position of the two nodes in the optimalToCurrent array		
		optimalToCurrent[rightNode.getOptimalPosition()]--;
		optimalToCurrent[leftNode.getOptimalPosition()]++;
			
		rightNode.decCurrentPosition();
		leftNode.incCurrentPosition();
	
		//switch the position of the two nodes in the array
		nodes[pos-1] = rightNode;
		nodes[pos] = leftNode;
		

		
		return pos-1;
	}
	
	
	/** 
	 * Move the specified node one position to the right
	 * @param pos position of the node to be moved
	 * @return the new position of the node
	 */
	protected int moveNodeRight(int pos) {
		if (pos == getN() - 1) {
			//trying to move node out of the right end of the list
			return pos;
		}
		
		Node leftNode = nodes[pos];
		Node rightNode = nodes[pos+1];
		
		//switch the position of the two nodes in the optimalToCurrent array		
		optimalToCurrent[leftNode.getOptimalPosition()]++;
		optimalToCurrent[rightNode.getOptimalPosition()]--;
			
		
		leftNode.incCurrentPosition();
		rightNode.decCurrentPosition();
			
		//switch the position of the two nodes in the array
		nodes[pos+1] = leftNode;
		nodes[pos] = rightNode;
		
		return pos+1;
	}

	/**
	 * Inform the controller about a new request. This updates some internal values and issues the rearrangement strategy.  
	 * @param posS position of the source node
	 * @param posD position of the destination node
	 */
	protected void processRequest(Request request) {
		//retrieve current positions from optimal positions
		int posS = getCurrentPosition(request.getPosS());
		int posD = getCurrentPosition(request.getPosD());
		//System.out.println("Receiving request: " + posS + " to " + posD);
		
		//update the cost
		cost += Math.abs(posS-posD);
		optimalCost += getDistanceInOptimal(posS, posD);
		
		//update the number of rounds
		elapsedRounds++;
		
		//inform the nodes
		nodes[posS].incSentRequests();
		nodes[posD].incReceivedRequests();
		
		//execute the rearrangement strategy		
		rearrange(posS, posD);
		
		lastRequest = request;
	}
	
	protected int getCurrentPosition(int optimalPosition) {
		return optimalToCurrent[optimalPosition];
	}
	
	/**
	 * Rearrange the nodes based on the implemented rearrangement strategy
	 * @param posS position of the source node
	 * @param posD position of the destination node
	 */
	protected abstract void rearrange(int posS, int posD);

	/**
	 * Create a gnuplot compatible .dat file for the current situation
	 * @return the contents of the file as a string
	 */
	public String createCurrentRoundDat() {
		String result = "#Node no \t sentRequests \t receivedRequests \t distanceToOptimalPosition\n";
		for (int i = 0; i < nodes.length; i++) {
			result += nodes[i].getOptimalPosition()+"\t"
			  +nodes[i].getSentRequests()+"\t"
			  +nodes[i].getReceivedRequests()+"\t"
			  +Math.abs(nodes[i].getCurrentPosition()-nodes[i].getOptimalPosition())+"\n";
		}
		return result;
	}

	/**
	 * @return the name of the strategy
	 */
	abstract public String getName();
	
	
	
}
