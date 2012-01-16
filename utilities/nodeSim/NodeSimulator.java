/**
 * 
 */
package nodeSim;

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
//		createStatistics4();
		
		System.out.println("Finished execution!");		
	}
	
	/**
	 * Simulates a strategy with a given number of nodes and different numbers of requests.
	 * The data created this way was used for the boxplot diagrams 2.1,2.2,2.4-2.7,2.9-2.12
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
	 * The data created this way was used for the boxplot diagram 2.8
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
	 *	The summary file showing the developing of phi was used for figure 2.3   
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
	 * Simulates the randomized version of strategy C (C') with the counter-example described in 2.4.3
	 */
	static void createStatistics4() {
		int numberOfNodes = 100; //parameter
		String dirPrefix = "counterExampleSimulations_"; //parameter
		
		String strategy = "CR";		
		String regretsDat = "";		
		String strategyName = "Strategy"+strategy;
		
		ArrayBlockingQueue<Request> requests = new ArrayBlockingQueue<Request>(100*numberOfNodes);		
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < numberOfNodes; j+=2)
				requests.add(new Request(j,j+1));
		}
		
		int[] initArrangement = new int[numberOfNodes];
		int x = 0;
		for (int i = 0; i < numberOfNodes - 1; i+=2)
			initArrangement[x++] = i;
		for (int i = numberOfNodes - 1; i > 0; i-=2)
			initArrangement[x++] = i;
		
		Input input = new Input(numberOfNodes, requests, initArrangement);
		
		SimulationManager simManager = new SimulationManager(dirPrefix+strategyName, true);

		for (int sim = 0; sim < 100; sim++ )
			regretsDat += simManager.runInput(input, false, "CounterexampleCheck.csv", true, strategy) + " ";
		
		Helper.createDir("simulations/"+dirPrefix+strategyName);
		Helper.saveStringToTextFile("simulations/"+dirPrefix+strategyName+"/", strategyName+"Boxplot"+numberOfNodes+".dat", regretsDat);
	}
	

}
