/**
 * 
 */
package nodeSim;

import java.io.BufferedWriter;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.io.FileWriter;
import java.io.IOException;

/**
 * Class for creating the analysis in the gnuplot format.
 * This class not only creates the data files, but also the .gnu scripts to be run by gnuplot. 
 *
 */
public class AnalysisCreator {
	private final static String GNUPLOT_TERM = "pdf";
	
	//the controller used for the current simulation
	protected SimulationController controller;

	//the directory to store the simulations in
	protected String directoryName;
	
	//the number of the current simulation
	protected int simulationNumber;
	
	protected class RoundResult {
		Integer phi;
		Integer cost;
		Integer optimalCost;
	};

	//array for storing the round results (for the summary)
	protected RoundResult[] roundResults;
	
	//maximum Phi value (for the summary)
	protected int maxPhi = 0;
	
	//maximum new cost (for the summary)
	protected int maxNewCost = 0;
	
	protected BufferedWriter sequenceTextFileOut;
	
	/**
	 * Constructor.
	 * @param controller SimulationController of the current simulation
	 * @param directoryName directory to store the simulation results in
	 * @param numberOfRounds total number of rounds simulated 
	 * @param simulationNumber the number of the current simulation
	 */
	AnalysisCreator(SimulationController controller, String directoryName, int numberOfRounds, int simulationNumber) {
		this.controller = controller;
		this.directoryName = directoryName;
		this.simulationNumber = simulationNumber;
		roundResults = new RoundResult[numberOfRounds+1];
		for (int i = 0; i < roundResults.length; i++) {
			roundResults[i] = new RoundResult();
		}
		
		try {
		    sequenceTextFileOut = new BufferedWriter(
		    		new FileWriter(directoryName+"/"+getSubdirectoryName() +"/"+ "sequence_" + simulationNumber + "_" + controller.getName() + ".txt")
		    );
		    sequenceTextFileOut.write("sequence text file - " + simulationNumber + " - " + controller.getName() + "\n" );
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Closes the text file writer for the sequence text file
	 */
	private void finish() {
	    try {
	    	sequenceTextFileOut.write("###\n");
			sequenceTextFileOut.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/**
	 * Saves the current round by creating the .gnu and .dat file
	 * @param controller the controller with the current round
	 * @param filename the name of the file to be saved to
	 */
	public void saveCurrentRound(String simulationName, boolean saveEachStep) {
		//store the information in the array
		roundResults[controller.getElapsedRounds()].cost = controller.getCost();
		roundResults[controller.getElapsedRounds()].optimalCost = controller.getOptimalCost();
		roundResults[controller.getElapsedRounds()].phi = controller.getPotential();
		if (controller.getPotential() > maxPhi) 
			maxPhi = controller.getPotential();
		if (controller.getElapsedRounds() > 0 && controller.getCost()-roundResults[controller.getElapsedRounds()-1].cost > maxNewCost)
			maxNewCost = controller.getCost()-roundResults[controller.getElapsedRounds()-1].cost;
		
		String seqLine = getSeqLine();
		sequenceTextFileAddLine(seqLine);
		
		if (saveEachStep) {
			//save .gnu file
			String filename = simulationName + "_" + new DecimalFormat("000000").format(controller.getElapsedRounds());
			String gnuFile = createCurrentRoundGnu(controller, filename );
		
			saveStringToTextFile(filename+".gnu", gnuFile, false);
			
			
			//save .dat file
			String dataFile = controller.createCurrentRoundDat();
			saveStringToTextFile(filename+".dat", dataFile, false);
		}
		
	}

	/**
	 * @return line with the current potential, current regret, last request and current arrangement
	 */
	public String getSeqLine() {
		String seqLine = Helper.formatString("000", controller.getPotential()) + " | ";				

		
		if (controller.getElapsedRounds() > 0) {
			seqLine += NumberFormat.getInstance().format((roundResults[controller.getElapsedRounds()].optimalCost==0 ? 0.0 : 1.0*roundResults[controller.getElapsedRounds()].cost / roundResults[controller.getElapsedRounds()].optimalCost))+ " | ";
			
			seqLine += Helper.formatString("00", controller.getLastRequest().getPosS()) + " - "
					+ Helper.formatString("00", controller.getLastRequest().getPosD()) + " ||| ";
		}
		else {
			seqLine += Helper.formatString("00", 0) + " - " + Helper.formatString("00", 0) + " ||| "; 
		}
		
		for (Node node: controller.getCurrentNodes()) {
			seqLine += Helper.formatString("00", node.optimalPosition) + " ";
		}
		return seqLine;
	}

	/**
	 * Create the gnu file for the current round analysis
	 * @param controller the simulationController
	 * @param filename the desired name of the file (without .svg)
	 * @return
	 */
	public static String createCurrentRoundGnu(SimulationController controller, String filename) {
		String result = "";
		result += "#"+"\n";
		result += "set term svg"+"\n";
		result += "set out \"" + filename + ".svg\""+"\n";
		result += "set title \"Situation in round " + controller.getElapsedRounds() +"\\n";
		result += "Phi = "+ controller.getPotential() + "\\n";
		result += "Cost = " + controller.getCost() + "\\n";
		if( controller.getLastRequest() != null)
			result += "Last request = " + controller.getLastRequest().getPosS() + "-" + controller.getLastRequest().getPosD();
		result += "\n";
		result += "set auto x" + "\n";
		result += "set yrange [0:" + controller.getNumberOfRounds()/controller.getN() * 2+ "]" + "\n";
		result += "set style data histogram" + "\n";
		result += "set style histogram cluster gap 1" + "\n";
		result += "set style fill solid noborder" + "\n";
		result += "set boxwidth 0.92" + "\n";
		result += "plot '" + filename + ".dat' using 2:xtic(1) t \"sentRequests\", '' using 3 t \"receivedRequests\", '' using 4 t \"distanceToOptimal\"" +"\n";
		result += "#";
		
		return result;
	}

	/**
	 * save a summary (diagram which shows phi and cost for each or some round)
	 * @param simulationName file name will be <simulationName>_summaryX.* 
	 */
	public void saveSummary(String simulationName) {
		String filenameprefix = simulationName+"_";
		
		saveSummary1(filenameprefix);
		//saveSummary2(filenameprefix);		
		saveSummary3(filenameprefix);		
		saveSummary4(filenameprefix);
		
		//close the text file writer
		finish();

	}

	/**
	 * Save the phi summary.
	 */
	protected void saveSummary1(String filenameprefix) {
		String filename = filenameprefix + "summary1";
		//save first .gnu file
		String gnuFile1 = createSummaryGnu(
				filename,
				"Phi",
				0,
				maxPhi*1.1,
				getStepLength(),
				new String[] { "Phi" }
		); 
		saveStringToTextFile(filename+".gnu", gnuFile1, true);
	
		//save first .dat file
		String dataFile1 = createSummaryDat( createSummaryTable1() );
		saveStringToTextFile(filename+".dat", dataFile1, true);
	}

	/**
	 * Save the summary of the new cost per round
	 */
	protected void saveSummary2(String filenameprefix) {
		String filename = filenameprefix + "summary2";
		//save second .gnu file
		String gnuFile2 = createSummaryGnu(
				filename,
				"New cost per Round (and new optimal cost)",
				0,
				maxNewCost*1.1,
				getStepLength(),
				new String[] { "New cost", "New optimal cost" }
		);
		saveStringToTextFile(filename+".gnu", gnuFile2, true);
	
		//save second .dat file
		String dataFile2 = createSummaryDat( createSummaryTable2() );
		saveStringToTextFile(filename+".dat", dataFile2, true);
	}

	/**
	 * Save the summary of the total cost 
	 */
	protected void saveSummary3(String filenameprefix) {
		String filename = filenameprefix + "summary3";
		//save third .gnu file
		String gnuFile3 = createSummaryGnu(
				filename,
				"Total Cost (and total optimal cost)",
				0,
				controller.getCost() * 1.1,
				getStepLength(),
				new String[] { "Cost" , "OptimalCost" }
		);
		
		saveStringToTextFile(filename+".gnu", gnuFile3, true);
	
		//save third .dat file
		String dataFile3 = createSummaryDat(createSummaryTable3());
		saveStringToTextFile(filename+".dat", dataFile3, true);
	}

	/**
	 * The the summary of the regret.
	 */
	protected void saveSummary4(String filenameprefix) {
		String filename = filenameprefix + "summary4";
		//save first .gnu file
		String gnuFile4 = createSummaryGnu(
				filename,
				"Regret",
				1,
				3 + controller.getN()/10,
				getStepLength(),
				new String[] { "Regret" }
		); 
		saveStringToTextFile(filename+".gnu", gnuFile4, true);
	
		//save first .dat file
		String dataFile4 = createSummaryDat(createSummaryTable4());
		saveStringToTextFile(filename+".dat", dataFile4, true);
	}

	/**
	 * Creates a .dat file for the summary diagram based on the contents of the table 	
	 * @return the content of the .dat file
	 */	
	protected static String createSummaryDat( String[][] resultTable ) {
		int numberOfRounds = resultTable.length - 1;
		String result = "#round number";
		//read the caption from the last table entry
		for (int i = 0; i < resultTable[numberOfRounds].length; i++) {
			result += "\t" + resultTable[numberOfRounds][i];
		}
		result += "\n";
		
		int stepLength = 1;
		
		int i;
		for (i = 0; i < numberOfRounds; i+= stepLength) {
			result += i;
			for (int j = 0; j < resultTable[i].length; j++) {
				result += "\t" + resultTable[i][j];
			}
			result += "\n";
		}
		
		if (i - stepLength != numberOfRounds - 1) {
			result += numberOfRounds -1;
			for (int j = 0; j < resultTable[numberOfRounds - 1].length; j++) {
				result += "\t" + resultTable[numberOfRounds - 1][j];
			}
		}
		
		return result;
	}

	protected static String createSummaryGnu(String filename, String title, double minYRange, double maxYRange, int stepLength, String[] captions) {
		if (maxYRange == 0)
			maxYRange = 1;
		
		String result = "";
		result += "#"+"\n";
		result += "set term " + GNUPLOT_TERM + "\n";
		result += "set out \"" + filename + "." + GNUPLOT_TERM + "\""+"\n";
		result += "set title \""+ title +"\"\n";
		result += "set auto x" + "\n";
		result += "set yrange [" + minYRange + ":" + maxYRange + "]" + "\n";
		result += "set style data histogram" + "\n";
		result += "set xtics font \"Arial,8\"" + "\n";
		result += "set xtic rotate by -45 scale 0" + "\n";
		result += "set style histogram cluster gap 1" + "\n";
		result += "set style fill solid noborder" + "\n";
		//result += "set boxwidth 0.92" + "\n";
		result += "everyxth(col) = (int(column(col)) % "+ stepLength + " == 0) ? stringcolumn(1) : \"\"\n";
		result += "plot '" + filename + ".dat' using 2:xtic(everyxth(1)) t \""+  captions[0] +"\" lc rgb \"dark-green\"";
		if (captions.length > 1)
			result += ", '' using 3 t \""+captions[1]+"\" lc rgb \"blue\" ";
		
		result += "\n";
		result += "#";
		
		return result;
	}	
	
	protected int getStepLength() {
		int stepLength = (int) Math.ceil(roundResults.length / 10);
		return stepLength;
	}		
	
	
	/**
	 * Saves the content in a text file.
	 */
	private void saveStringToTextFile(String filename, String content, boolean summary) {
		try {
		    BufferedWriter out = new BufferedWriter(
		    		new FileWriter(directoryName+"/"+(summary? "summary":getSubdirectoryName()) +"/"+filename));
		    out.write(content);
		    out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * @return name of the sub directory where the current round files are stored in
	 */
	private String getSubdirectoryName() {
		return new DecimalFormat("0000").format(simulationNumber)+ "/"+ controller.getName();
	}
	
	/**
	 * Add a line to the text file storing the sequence of list states.
	 */
	private void sequenceTextFileAddLine(String line) {
		try {
			sequenceTextFileOut.write(line + "\n");
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Create the summary table for the diagram type 1 (Phi)
	 * @return the table with the phi values
	 */
	protected String[][] createSummaryTable1() {
		final int COL_COUNT = 1;
		
		//initialize array
		String[][] result = new String[roundResults.length + 1][];
		for (int i = 0; i < roundResults.length + 1; i++) {
			result[i] = new String[COL_COUNT];
		}
		
		//caption
		result[roundResults.length][0] = "Phi";
	
		//values
		for (int i = 0; i < roundResults.length; i++) {
			result[i][0] = roundResults[i].phi.toString();
		}
	
		return result;
	}

	/**
	 * Creates the table for the summary diagram type 2 (New cost) 	
	 * @return the content of the .dat file
	 */
	protected String[][] createSummaryTable2() {
		final int COL_COUNT = 2;
		
		//initialize array
		String[][] result = new String[roundResults.length + 1][];
		for (int i = 0; i < roundResults.length + 1; i++) {
			result[i] = new String[COL_COUNT];
		}
		
		//caption
		result[roundResults.length][0] = "Newly generated cost";
		result[roundResults.length][1] = "Newly generated (optimal) cost";
	
		//values
		result[0][0] = roundResults[0].cost.toString();
		for (int i = 1; i < roundResults.length; i++) {
			result[i][0] = ""+(roundResults[i].cost-roundResults[i-1].cost);
			result[i][1] = ""+(roundResults[i].optimalCost-roundResults[i-1].optimalCost);
		}
			
		return result;
		
	}

	/**
	 * Creates the .dat file for the summary diagram type 3 (total cost) 	
	 * @return the content of the .dat file
	 */
	protected String[][] createSummaryTable3() {
		final int COL_COUNT = 2;
		
		//initialize array
		String[][] result = new String[roundResults.length + 1][];
		for (int i = 0; i < roundResults.length + 1; i++) {
			result[i] = new String[COL_COUNT];
		}
		
		//caption
		result[roundResults.length][0] = "Total cost";
		result[roundResults.length][1] = "Optimal cost";
	
		//values
		for (int i = 0; i < roundResults.length; i++) {
			result[i][0] = ""+roundResults[i].cost;
			result[i][1] = ""+roundResults[i].optimalCost;
		}
		
		return result;
	}

	/**
	 * Creates the .dat file for the summary diagram type 4 (Regret) 	
	 * @return the content of the .dat file
	 */
	protected String[][] createSummaryTable4() {
		final int COL_COUNT = 1;
		
		//initialize array
		String[][] result = new String[roundResults.length + 1][];
		for (int i = 0; i < roundResults.length + 1; i++) {
			result[i] = new String[COL_COUNT];
		}
		
		//caption
		result[roundResults.length][0] = "Regret";

		//values
		for (int i = 0; i < roundResults.length; i++) {
			result[i][0] = (roundResults[i].optimalCost==0 ? "0.0" : Double.toString(1.0*roundResults[i].cost / roundResults[i].optimalCost));
		}				
		return result;
	}
	
}
