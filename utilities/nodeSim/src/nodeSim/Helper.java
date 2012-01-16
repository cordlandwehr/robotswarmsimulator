package nodeSim;

import java.io.BufferedInputStream;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.util.Arrays;

/**
 * Helper class containing useful static methods
 *
 */
class Helper {
	static String readFileAsString(String filePath) throws java.io.IOException{
	    byte[] buffer = new byte[(int) new File(filePath).length()];
	    BufferedInputStream f = null;
	    try {
	        f = new BufferedInputStream(new FileInputStream(filePath));
	        f.read(buffer);
	    } finally {
	        if (f != null) try { f.close(); } catch (IOException ignored) { }
	    }
	    return new String(buffer);
	}
	
	public static void saveStringToTextFile(String directoryName, String filename, String content) {
		try {
		    BufferedWriter out = new BufferedWriter(
		    		new FileWriter(directoryName+"/"+filename));
		    out.write(content);
		    out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	public static String formatString(String pattern, Object obj)  {
		return new DecimalFormat(pattern).format(obj);
	}

	/**
	 * outputs the line for the regret data file
	 * @param regrets the different regret values of the current simulation
	 * @return the regret data file line
	 */
	public static String getRegretDatLine(double[] regrets) {
		Arrays.sort(regrets);
		double min = regrets[0];
		
		int lowerQ = (int) Math.round(regrets.length*0.25);
		if (lowerQ >= regrets.length)
			lowerQ = regrets.length-1;
		
		double lQuartile = regrets[lowerQ];
		double mean = 0;
		
		for (int i = 0; i < regrets.length; i++) {
			mean += regrets[i]; 
		}
		mean = mean / regrets.length;

		int upperQ = (int) Math.round(regrets.length*0.75);
		if (upperQ >= regrets.length)
			upperQ = regrets.length-1;
		
		double uQuartile = regrets[upperQ];
		double max = regrets[regrets.length-1];
		
		return min+" "+lQuartile+" "+mean+" "+uQuartile+" "+max ;
		
	}
	
	/**
	 * Creates the specified directory
	 * @param directoryName the directory to be created
	 */
	public static void createDir(String directoryName) {
		boolean success = (new File(directoryName)).mkdirs();
		if (success) {
		  System.out.println("Directory: " + directoryName + " created");
		} else {
			System.out.println("Directory: " + directoryName + " NOT created");
			//delete contents of the directory:
			/*String[] children = new File(directoryName).list();
	        for (int i=0; i<children.length; i++) {
	        	if (! new File(directoryName, children[i]).delete()) {
	        			System.err.println("Could not delete "+directoryName+"/"+children[i]);
	        	}	        		
	        }*/
		}
	}	
	
}
