package bgu.spl.mics.application.passiveObjects;

/**
 * Passive data-object representing a information about an agent in MI6.
 * You must not alter any of the given public methods of this class. 
 * <p>
 * You may add ONLY private fields and methods to this class.
 */
public class Agent {

	String SerialNumber;
	String name;
	Boolean available;

	public Agent(String serialNumber,String name){
       this.SerialNumber=serialNumber;
       this.name=name;
       available=true;
	}
	/**
	 * Sets the serial number of an agent.
	 */
	public void setSerialNumber(String serialNumber) {
		this.SerialNumber=serialNumber;
	}

	/**
     * Retrieves the serial number of an agent.
     * <p>
     * @return The serial number of an agent.
     */
	public String getSerialNumber() {
		return this.SerialNumber;
	}

	/**
	 * Sets the name of the agent.
	 */
	public void setName(String name) {
		this.name=name;
	}

	/**
     * Retrieves the name of the agent.
     * <p>
     * @return the name of the agent.
     */
	public String getName() {
		return name;
	}

	/**
     * Retrieves if the agent is available.
     * <p>
     * @return if the agent is available.
     */
	public boolean isAvailable() {
		return available;
	}

	/**
	 * Acquires an agent.
	 */
	public void acquire(){
		available=false;
	}

	/**
	 * Releases an agent.
	 */
	public void release(){
		available=true;
	}
}
