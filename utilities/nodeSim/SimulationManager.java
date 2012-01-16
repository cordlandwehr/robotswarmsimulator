/**
 * 
 */
package nodeSim;

import java.io.File;
import java.io.IOException;
import java.text.DecimalFormat;

import nodeSim.containers.Input;
import nodeSim.controllers.StrategyAController;
import nodeSim.controllers.StrategyBController;
import nodeSim.controllers.StrategyCController;
import nodeSim.controllers.StrategyCRController;
import nodeSim.controllers.StrategyDController;
import nodeSim.controllers.StrategyEController;


/**
 * Responsible for creating and running the simulations repeatedly and creating the evaluation.
 * @author asetzer
 */
class SimulationManager {
	/** Name of this simulation */
	protected String simulationName;
	
	protected final static String CSV_NAME = "input.csv";
	
	/** The number of simulations yet run by this SimulationManager */
	protected int runSimulations = 0;

	/** Used for better evaluation - seqLines[instance][strategy] */
	private String[][] seqLines; 
	
	private boolean ultraSilent;
	
	/**
	 * Constructor. 
	 * @param simulationName a name for this simulation (used for the directory structure)
	 */
	SimulationManager(String simulationName, boolean ultraSilent) {
		this.simulationName = simulationName;
		this.ultraSilent = ultraSilent;
	}
	
	/**
	 * runs several simulation based on generated inputs in series and creates an evaluation using average values
	 * @param numberOfSimulations number of simulations to generate and run
	 * @param numberOfNodes number of nodes in the simulation instances
	 * @param numberOfRequests number of requests to be generated
	 * @param distribution the probability distribution to use (see InputGenerator)
	 * @param saveEachStep defines whether the state of the list after each step should be saved in a .dat file
	 * @param generateInitialArrangement defines whether the initial arrangement should be generated randomly or whether it should be the optimal arrangement   
	 * @param strategies the strategies to use
	 * @return the resulting line for the regret data file
	 */
	String generateAndRunMultipleInputs(int numberOfSimulations, int numberOfNodes, int numberOfRequests, int distribution, boolean saveEachStep, boolean generateInitialArrangement, String strategies) {
		if (!ultraSilent)
			seqLines = new String[numberOfSimulations][];
		double[] regrets = new double[numberOfSimulations];
		
		for (int i = 0; i < numberOfSimulations; i++) {
			regrets[i] = generateAndRunInput(numberOfNodes, numberOfRequests, distribution, saveEachStep, generateInitialArrangement, strategies);
		}
			
		if (!ultraSilent) {
			saveSummaryTextSortedByInstances(getDirectoryName(numberOfNodes, numberOfRequests, true), numberOfSimulations);
			saveSummaryTextSortedByStrategies(getDirectoryName(numberOfNodes, numberOfRequests, true), numberOfSimulations);
		}
		
		return Helper.getRegretDatLine(regrets);
	}

	/**
	 * Saves a summary of all simulations in a text file.
	 * Strategies are grouped for each instance. 
	 */
	protected void saveSummaryTextSortedByInstances(String directory, int numberOfSimulations) {
		//create summaryText sorted by instances
		String summaryText = "";
		for( int i = 0; i < numberOfSimulations; i++) {
			for (int j = 0; j < seqLines[i].length; j++)
				summaryText += seqLines[i][j] + "\n";
			summaryText += "\n";
		}
		Helper.saveStringToTextFile(directory, "summaryTextSortedByInstances.txt", summaryText);
	}
	
	/**
	 * Saves a summary of all simulations in a text file.
	 * Instances are grouped for each strategy.
	 */
	protected void saveSummaryTextSortedByStrategies(String directory, int numberOfSimulations) {
		if (seqLines.length > 0)
		{
			//create summaryText sorted by instances
			String summaryText = "";
			for( int i = 0; i < seqLines[0].length; i++) {
				for (int j = 0; j < seqLines.length; j++)
					summaryText += seqLines[j][i] + "\n";
				summaryText += "\n";
			}
			Helper.saveStringToTextFile(directory, "summaryTextSortedByStrategies.txt", summaryText);
		}
	}

	/**
	 * Generates an input and runs a simulation based on it
	 * @param numberOfNodes the number of nodes
	 * @param numberOfRequests the number of requests to be generated
	 * @param distribution the probability distribution to use (see InputGenerator)
	 * @param saveEachStep defines whether the state of the list after each step should be saved in a .dat file
	 * @param generateInitialArrangement defines whether the initial arrangement should be generated randomly or whether it should be the optimal arrangement   
	 * @param strategies the strategies to use 
	 * @return the regret at the end of the simulation
	 */
	double generateAndRunInput(int numberOfNodes, int numberOfRequests, int distribution, boolean saveEachStep, boolean generateInitialArrangement, String strategies) {
		if ((!ultraSilent) && (seqLines == null))
			seqLines = new String[1][];		
		Input input = InputGenerator.generateInput(numberOfNodes, numberOfRequests, distribution, generateInitialArrangement);
		return runInput(input, true, CSV_NAME, saveEachStep, strategies);		
	}
	
	/**
	 * Loads an input from file and runs the corresponding simulations
	 * @param directoryName name of the directory the instances are stored in
	 * @param saveEachStep defines whether the state of the list after each step should be saved in a .dat file
	 * @param strategies the strategies to use
	 */
	void loadAndRunMultipleInputs(String directoryName, boolean saveEachStep, String strategies) {
		int numberOfSimulations = getFirstFreeSimulationNumber(directoryName)-1;
		if (!ultraSilent)
			seqLines = new String[numberOfSimulations][];
		for (int i = 0; i < numberOfSimulations; i++) {
			loadAndRunInput(directoryName, i+1, saveEachStep, strategies);
		}
		
		if (!ultraSilent) {
			saveSummaryTextSortedByInstances("simulations/"+simulationName, numberOfSimulations);
			saveSummaryTextSortedByStrategies("simulations/"+simulationName, numberOfSimulations);
		}
	}
	
	/**
	 * Loads an input from file and runs the corresponding simulation
	 * @param directoryName name of the directory the instance is stored in
	 * @param simulationNumber the number of the current simulation
	 * @param saveEachStep defines whether the state of the list after each step should be saved in a .dat file
	 * @param strategies the strategies to use

	 */
	void loadAndRunInput(String directoryName, int simulationNumber, boolean saveEachStep, String strategies) {
	
		String csv = "";
		try {
			csv = Helper.readFileAsString(getCurRoundDirectoryName(directoryName, simulationNumber)+"/"+CSV_NAME);
		} catch (IOException e) {
			e.printStackTrace();
		}
		Input input = new Input(csv);
		runInput(input, false, CSV_NAME, saveEachStep, strategies);
		
	}

	/**
	 * runs the simulation with the given input and creates the analysis
	 * @param input the input to simulate with
	 * @param generated defines whether the input is generated (true) or loaded from file (false)
	 * @param cvsName the name of the cvs file the input is loaded from
	 * @param saveEachStep defines whether the state of the list after each step should be saved in a .dat file
	 * @param strategies the strategies to use
	 * @return the regret at the end of the simulation
	 */
	double runInput(Input input, boolean generated, String csvName, boolean saveEachStep, String strategies) {	
		//create the directory for the current simulation
		String directoryName = getDirectoryName(input.getNumberOfNodes(), input.getTotalNumberOfRequests(), generated);	
		int simulationNumber = getFirstFreeSimulationNumber(directoryName);
		
		String curRoundDirectoryName = getCurRoundDirectoryName(directoryName, simulationNumber);
		if (!ultraSilent) {
			Helper.createDir(curRoundDirectoryName);
			Helper.createDir(directoryName+"/summary");
		}
		
	
		if (!ultraSilent) {
			Helper.saveStringToTextFile(curRoundDirectoryName, csvName, input.toCSV());
		}

		String csv = input.toCSV();
			
		//create the controllers
		SimulationController[] controllers;
		if (strategies == "ABCDE")
			controllers = new SimulationController[]{ 
				new StrategyAController(new Input(csv)),
				new StrategyBController(new Input(csv)),
				new StrategyCController(new Input(csv)),
				new StrategyDController(new Input(csv)),
				new StrategyEController(new Input(csv))
			};
		else if (strategies == "A")
			controllers = new SimulationController[]{ 
				new StrategyAController(new Input(csv))
			};
		else if (strategies == "B")
			controllers = new SimulationController[]{ 
				new StrategyBController(new Input(csv))
			};
		else if (strategies == "C")
			controllers = new SimulationController[]{ 
				new StrategyCController(new Input(csv))
			};
		else if (strategies == "D")
			controllers = new SimulationController[]{ 
				new StrategyDController(new Input(csv))
			};
		else if (strategies == "E")
			controllers = new SimulationController[]{ 
				new StrategyEController(new Input(csv))
			};
		else if (strategies == "CR")
			controllers = new SimulationController[]{ 
				new StrategyCRController(new Input(csv))
			};		
		else
			controllers = new SimulationController[]{};						
	
		if (!ultraSilent)
			seqLines[runSimulations] = new String[controllers.length];
		
		for( int i = 0; i < controllers.length; i++) {
			if (!ultraSilent) {
				//Input curInput = new Input(csv);
				String resultDir = curRoundDirectoryName+"/"+controllers[i].getName(); 
				
				Helper.createDir(resultDir);
			}

			AnalysisCreator analysisCreator = null;
			if (!ultraSilent) {
				//initialize the AnalysisCreator
				analysisCreator = new AnalysisCreator(controllers[i],directoryName,controllers[i].getNumberOfRounds(),simulationNumber);
		
				//save the initial state
				analysisCreator.saveCurrentRound(controllers[i].getName(), true);
			}
		
			for( int j = 0; j <= controllers[i].getNumberOfRounds(); j++) {
				controllers[i].runOneStep();
				if (!ultraSilent) {
					if (j == controllers[i].getNumberOfRounds())
						analysisCreator.saveCurrentRound(controllers[i].getName(), true);
					else
						analysisCreator.saveCurrentRound(controllers[i].getName(), saveEachStep);
				}
			}
			
			if (!ultraSilent) {
				analysisCreator.saveSummary(controllers[i].getName() + "_" + new DecimalFormat("0000").format(simulationNumber));
			
				seqLines[runSimulations][i] = controllers[i].getName() + "("+ simulationNumber + ")" + " | " + analysisCreator.getSeqLine();
			}
			
			//System.out.println("Generated cost: " + controller.getCost());
		}
		runSimulations++;
		return 1.0*controllers[0].getCost() / controllers[0].getOptimalCost();
	}

	/**
	 * Returns the formatted name of the directory to stores the simulations in
	 * @param numberOfNodes the number of nodes
	 * @param numberOfRequests the number of requests
	 * @param generated whether the input was generated (and not loaded from file)
	 * @return the formatted directory name
	 */
	protected String getDirectoryName(int numberOfNodes, int numberOfRequests, boolean generated) {
		String genString = "";
		if (generated) {
			String nodeCount = new DecimalFormat("000").format(numberOfNodes);
			String roundCount = new DecimalFormat("0000").format(numberOfRequests);
			genString = "_generated_" + nodeCount + "_" + roundCount;
		}
					
		String directoryName = "simulations/"+ simulationName+genString;
		return directoryName;
	}

	/**
	 * @returnthe formatted name of the directory to stores the simulation of the current round in
	 */
	private String getCurRoundDirectoryName(String directoryName,
			int simulationNumber) {
		return directoryName+"/"+new DecimalFormat("0000").format(simulationNumber);
	}



	
	/**
	 * Get the first possible simulation number that has not been used already (determined by the directory name)
	 * @param directoryName the directory
	 * @return the first possible simulation number that has not been used already
	 */
	private int getFirstFreeSimulationNumber(String directoryName) {

        for (int i = 1; i < 9999; i++) {
        	if (!new File(directoryName, new DecimalFormat("0000").format(i)).exists())
        		return i;
        }
        return -1;
	}
	
	/*private void emptyDirectory(String directoryName) {
		String[] children = new File(directoryName).list();
        for (int i=0; i<children.length; i++) {
        	if (! new File(directoryName, children[i]).delete()) {
        			System.err.println("Could not delete "+directoryName+"/"+children[i]);
        	}	        		
        }
	}*/
	
	
}
