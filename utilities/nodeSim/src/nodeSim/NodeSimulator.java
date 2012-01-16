/**
 * 
 */
package nodeSim;

import java.util.ArrayList;
import java.util.Vector;
import java.util.concurrent.ArrayBlockingQueue;

import nodeSim.containers.Input;
import nodeSim.containers.Request;



/**
 * Main class
 * @author Alexander Setzer
 */
public class NodeSimulator {
	/**
	 * The main method.
	 * Uncoment the lines to start certain simulations.
	 */
	public static void main(String[] args) {
//		createStatistics1();
//		createStatistics2();
//		createStatistics3();
		createStatistics4();
		
		System.out.println("Finished execution!");		
	}
	
	/**
	 * Simulates a strategy with a given number of nodes and different numbers of requests.
	 */
	static void createStatistics1() {
		String strategy = "C"; //parameter
		String dirPrefix = "regretSimulationsByRequests_";  //parameter		
		int numberOfNodes = 10;
		int distribution = InputGenerator.DIST_NORMAL; //parameter
		
		String strategyName = "Strategy"+strategy;
		SimulationManager simManager = new SimulationManager(dirPrefix+strategyName, true);

		String regretsDat = "";
		
		for (int numberOfRequests = numberOfNodes*10; numberOfRequests < numberOfNodes*150; numberOfRequests+= numberOfNodes*10 ) {
			regretsDat += numberOfRequests + " " + simManager.generateAndRunMultipleInputs(100, numberOfNodes, numberOfRequests, distribution, false, true, strategy) + "\n";
			System.out.println("Finished simulation with "+ numberOfRequests +" requests.");
		}
		
		Helper.createDir("simulations/"+dirPrefix+strategyName);
		Helper.saveStringToTextFile("simulations/"+dirPrefix+strategyName+"/", strategyName+"Boxplot"+numberOfNodes+".dat", regretsDat);
		

	}

	/**
	 * Simulates a strategy with different numbers of nodes, each time generating 100*numberOfNodes requests.
	 * 
	 * Since this can take long, a backup is saved after a certain number of nodes has been simulated for the desired number of instances 
	 */
	static void createStatistics2() {
		String strategy = "C"; //parameter
		int maxNumberOfNodes = 900; //parameter		
		String dirPrefix = "regretSimulationsByNodes_"; //parameter
		int distribution = InputGenerator.DIST_NORMAL; //parameter
		int numberOfSimulations = 100; //parameter - number of simulations per node count

		
		String strategyName = "Strategy"+strategy;
		SimulationManager simManager = new SimulationManager(dirPrefix+strategyName, true);
		String regretsDat = "";
		
		Helper.createDir("simulations/"+dirPrefix+strategyName);
		for( int numberOfNodes = 100; numberOfNodes <= maxNumberOfNodes; numberOfNodes += 100) {
			regretsDat += numberOfNodes + " " + simManager.generateAndRunMultipleInputs(numberOfSimulations, numberOfNodes, numberOfNodes*100, distribution, false, true, strategy) + "\n";
			Helper.saveStringToTextFile("simulations/"+dirPrefix+strategyName+"/", strategyName+"Boxplot"+numberOfNodes+".dat", regretsDat);
			System.out.println("Finished simulation with "+ numberOfNodes +" nodes.");			
		}


		Helper.saveStringToTextFile("simulations/"+dirPrefix+strategyName+"/", strategyName+"Boxplot"+maxNumberOfNodes+".dat", regretsDat);
		
		
		
		
	}

	/**
	 * Simulates all strategies with a given number of nodes, a given number of requests and a given distribution.
	 * The number of instances generated and simulated can be specified, too.
	 * 
	 * The generated files contain 
	 *  - the state of the list in each round (sequence*.txt in the <number of simulation> subfolder)
	 *  - summary files displaying (for each strategy) the developing of phi, the generated cost (compared to the optimal cost) and the regret by rounds (subfolder summary)
	 *  - summary files displaying (for all strategies) the resulting values of phi, the regret and the final arrangement after the last simulation round of each of the simulation instances (summaryTextSortedByInstances.txt and summaryTextSortedByStrategies.txt)
	 *     
	 */
	static void createStatistics3() {
		String strategy = "ABCDE"; //parameter
	
		String dirPrefix = "completeSimulations_"; //parameter
		int distribution = InputGenerator.DIST_NORMAL; //parameter
		int numberOfSimulations = 2; //parameter
		int numberOfNodes = 20; //parameter
		int numberOfRequests = 1200; //parameter
		
		String strategyName = "Strategy"+strategy;
		SimulationManager simManager = new SimulationManager(dirPrefix+strategyName, false);

		simManager.generateAndRunMultipleInputs(numberOfSimulations, numberOfNodes, numberOfRequests, distribution, false, true, strategy);	
	}
	
	/**
	 * Simulates the go-to-the-middle strategy in the new round model
	 */
	static void createStatistics4() {
		int numberOfNodes = 50; //parameter
		int numberOfRounds = 1000; //parameter
		int numberOfSimulations = 1; //parameter
		String dirPrefix = "goToTheMiddleSimulations_"; //parameter
		
		String strategy = "GTTM";		
		String regretsDat = "";		
		String strategyName = "Strategy"+strategy;
		
		SimulationManager simManager = new SimulationManager(dirPrefix+strategyName, false, numberOfSimulations);
		
		ArrayBlockingQueue<Request> requests = new ArrayBlockingQueue<Request>(numberOfRounds*(2*numberOfNodes-2));		
		for (int i = 0; i < numberOfRounds; i++) {
			ArrayList<Integer> pairList = new ArrayList<Integer>();
			for (int j = 0; j < numberOfNodes; j++) {
				pairList.add(j*2);
				pairList.add(j*2+1);
			}
			
			for (int j = 0; j < 2*numberOfNodes; j++) {
				int selection = (int)(Math.random()*pairList.size());
				int pair = pairList.get(selection);
				pairList.remove(selection);				
				
				if (pair == 0 || pair == (numberOfNodes-1)*2+1) {
					//reflip
					continue;
				}
				
				if (pair % 2 == 0) { //pair is even --> request to the previous node
					requests.add(new Request(pair/2,pair/2-1));
				} else { //pair is odd --> request to the next node
					requests.add(new Request(pair/2,pair/2+1));
				}
				
				
			}
		}
		
		//generate the initial arrangement
		int[] initialArrangement = new int[numberOfNodes];
		//fill the help vector with all numbers from 0 to numberOfNodes - 1
		Vector<Integer> helpVector = new Vector<Integer>();
		for (int i = 0; i < numberOfNodes; i++) {
			helpVector.add(i);
		}
		
		//now pick one number each round
		for (int i = 0; i < numberOfNodes; i++) {
			int randomNumber = (int)(Math.random()*helpVector.size());//  (new Random()).nextInt(numberOfNodes-i);
			int nextNodeNumber = helpVector.get(randomNumber);
			helpVector.remove(randomNumber);
			initialArrangement[i] = nextNodeNumber;			
		}		
		
		
		Input input = new Input(numberOfNodes, requests, initialArrangement);
		
		regretsDat += simManager.runInput(input, false, "GoToTheMiddleCheck.csv", false, strategy) + " ";
			
		
		Helper.createDir("simulations/"+dirPrefix+strategyName);
		Helper.saveStringToTextFile("simulations/"+dirPrefix+strategyName+"/", strategyName+"Boxplot"+numberOfNodes+".dat", regretsDat);
	}
	

}
