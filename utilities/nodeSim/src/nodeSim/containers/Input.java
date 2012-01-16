package nodeSim.containers;

import java.util.Queue;
import java.util.concurrent.ArrayBlockingQueue;

/**
 * Represents a complete input for the simulator (including the queue of requests and the initial arrangement).
 */
public class Input {
	/**
	 * the number of nodes in this input instance
	 */
	protected int numberOfNodes;
	
	/**
	 * the number of requests this input instance had in the queue at creation time
	 */
	protected int totalNumberOfRequests;
	
	/** queue containing all the requests in the right order */
	Queue<Request> requests;

	/** the initial arrangement 
	 * initialArrangement[i] == j means that position i holds the node with optimal position j
	 */
	int[] initialArrangement;
	
	/**
	 * @param numberOfNodes the number of nodes in the current simulation 
	 * @param initialArrangement queue containing all the requests in the right order
	 * @param requests
	 */
	public Input(int numberOfNodes, Queue<Request> requests, int[] initialArrangement) {
		this.initialArrangement = initialArrangement;
		this.requests = requests;
		this.numberOfNodes = numberOfNodes;
		this.totalNumberOfRequests = requests.size();
	}
	
	/**
	 * Create input from file
	 * @param csv the contents of the .csv file storing the input
	 */
	public Input(String csv) {
		parseCSV(csv);
	}

	/**
	 * @return the numberOfNodes
	 */
	public int getNumberOfNodes() {
		return numberOfNodes;
	}

	/**
	 * @return the requests
	 */
	public Queue<Request> getRequests() {
		return requests;
	}
	
	/**
	 * @return the totalNumberOfRequests
	 */
	public int getTotalNumberOfRequests() {
		return totalNumberOfRequests;
	}

	/**
	 * @return the initialArrangement
	 */
	public int[] getInitialArrangement() {
		return initialArrangement;
	}

	/**
	 * Transforms the contents of the Input into a csv format, which is:
	 * <number of nodes> //e.g. 5
	 * <number of requests> //e.g. 10
	 * <initial arrangement> //e.g. 5,3,1,4,2
	 * <empty line>
	 * <list of requests> //e.g. 1,5
	 *                    //     2,4
	 *                    //     3,2
	 * @return
	 */
	public String toCSV() {
		String result = "";
		//write number of nodes
		result += numberOfNodes+"\n";
		
		//write number of requests
		result += requests.size()+"\n";
		
		//write initial arrangement followed by an empty line
		for (int i = 0; i < initialArrangement.length - 1; i++) {
			result += initialArrangement[i] + ",";
		}
		result += initialArrangement[initialArrangement.length-1] + "\n\n";
		
		//write list of requests
		Object[] requestArray = requests.toArray();
		for (int i = 0; i < requestArray.length; i++)
			result += ((Request) requestArray[i]).getPosS() + "," + ((Request) requestArray[i]).getPosD() + "\n"; 
		
		return result;
	}

	/**
	 * Sets the attribute values of this input based on the contents of the given csv file. 
	 * @param csv the content of the csv file
	 */
	protected void parseCSV(String csv) {
		//get the individual lines of the csv file
		String[] contents = csv.split("\n");
		
		//read number of nodes
		numberOfNodes = Integer.parseInt(contents[0]);
		
		//read number of requests
		int numberOfRequests = Integer.parseInt(contents[1]);
		
		//fill the array with the initial arrangements
		String[] initialArrangementStr = contents[2].split(",");
		initialArrangement = new int[numberOfNodes];
		for (int i = 0; i < numberOfNodes; i++) {
			initialArrangement[i] = Integer.parseInt(initialArrangementStr[i]);
		}
		
		//create the request queue and read the requests
		requests = new ArrayBlockingQueue<Request>(numberOfRequests);		
		for (int i = 4; i < contents.length; i++) {
			String[] currentRequest = contents[i].split(",");
			requests.add(new Request(Integer.parseInt(currentRequest[0]),Integer.parseInt(currentRequest[1])));			
		}
	}
}