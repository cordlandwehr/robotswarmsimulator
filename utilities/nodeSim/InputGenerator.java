/**
 * 
 */
package nodeSim;

import java.util.Random;
import java.util.Vector;
import java.util.concurrent.ArrayBlockingQueue;

import nodeSim.containers.Input;
import nodeSim.containers.Request;


/**
 * Generates the inputs for the simulations (random initial arrangement and random requests)
 */
class InputGenerator {
	final static int DIST_SIMPLE = 0;
	final static int DIST_NORMAL = 1;
	
	/**
	 * randomly generates an input for the simulation
	 * @param numberOfNodes the number of nodes
	 * @param numberOfRequests the number of requests to be generated
	 * @param distribution the probability distribution to use (DIST_NORMAL,DIST_EXPONENTIAL)
	 * @param generateInitialArrangement defines whether the initial arrangement should be generated randomly or whether it should be the optimal arrangement
	 * @return the generated input
	 */
	static Input generateInput(int numberOfNodes, int numberOfRequests, int distribution, boolean generateInitialArrangement ) {
		//generate an initial arrangement
		int[] initialArrangement = new int[numberOfNodes];

		if (generateInitialArrangement) {
			//fill the help vector with all numbers from 0 to numberOfNodes - 1
			Vector<Integer> helpVector = new Vector<Integer>(numberOfNodes);
			for (int i = 0; i < numberOfNodes; i++) {
				helpVector.add(i);
			}
			
			//now pick one number each round
			for (int i = 0; i < numberOfNodes; i++) {
				int randomNumber = (new Random()).nextInt(numberOfNodes-i);
				int nextNodeNumber = helpVector.get(randomNumber);
				helpVector.remove(randomNumber);
				initialArrangement[i] = nextNodeNumber;			
			}
		}
		else {
			for (int i = 0; i < numberOfNodes; i++) {
				initialArrangement[i] = i;
			}
		}
		
		
		return generateInput(numberOfNodes, numberOfRequests, distribution, initialArrangement);
	}

	/**
	 * randomly generates an input for the simulation
	 * @param numberOfNodes the number of nodes
	 * @param numberOfRequests the number of requests to be generated
	 * @param distribution the probability distribution to use (DIST_NORMAL,DIST_EXPONENTIAL)
	 * @param initialArrangement the initial arrangement
	 * @return the generated input
	 */
	static Input generateInput(int numberOfNodes, int numberOfRequests, int distribution, int[] initialArrangement) {
		Input result = null;

			ArrayBlockingQueue<Request> requests = new ArrayBlockingQueue<Request>(numberOfRequests);
			Random random1 = new Random();
			//Random random2 = new Random();
			Random[] randoms = new Random[numberOfNodes];
			for (int i = 0; i < numberOfNodes; i++)
				randoms[i] = new Random();
			
			//create the desired number of requests
			for( int i=0; requests.size() < numberOfRequests; i++) {
				//choose one source node
				int sourceNode = random1.nextInt(numberOfNodes);
				
				
				//choose one target node
				int destinationNode = -1;
				
				switch(distribution) {
				case DIST_NORMAL:

					double gaussianValue;
					//prohibit gaussianValue from being zero
					do {
						gaussianValue = randoms[sourceNode].nextGaussian();
					} while(gaussianValue == 0);
					
					//stretch gaussianValue
					final double STRETCH_FACTOR = 1.0;
					gaussianValue *= STRETCH_FACTOR;
					
					//find appropriate integer
					int diffToTargetNode;
					if (gaussianValue > 0)
						diffToTargetNode = (int) Math.ceil(gaussianValue);
					else
						diffToTargetNode = (int) Math.floor(gaussianValue);
					
					destinationNode = sourceNode + diffToTargetNode;
					break;
				case DIST_SIMPLE:
					destinationNode = sourceNode + (randoms[sourceNode].nextBoolean() ? 1 : -1);
					break;
				}
					
				if ((destinationNode < 0) || (destinationNode >= numberOfNodes)) 
					continue; //discard
				requests.add( new Request(sourceNode, destinationNode) );
				
			}
			
			result = new Input(numberOfNodes, requests, initialArrangement);

		return result;
	}
	
}
