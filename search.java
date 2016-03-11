/**
 * SearchResult stores a single searchresult.
 * 
 * @author CaylaR
 * 
 */
public class SearchResult implements Comparable<SearchResult> {

	private int frequency, position;
	private final String path;

	/**
	 * Constructor for the single searchresult
	 * 
	 * @param frequency
	 * @param position
	 * @param path
	 */
	public SearchResult(int frequency, int position, String path) {
		this.frequency = frequency;
		this.position = position;
		this.path = path;
	}

	/**
	 * Allows the frequency to be returned
	 * 
	 * @return
	 */
	public int getFrequency() {
		return frequency;
	}

	/**
	 * Allows the initial position to be returned
	 * 
	 * @return
	 */
	public int getPosition() {
		return position;
	}

	/**
	 * Allows the path to be returned as a string
	 * 
	 * @return
	 */
	public String getPath() {
		return path;
	}

	/**
	 * Returns the path, frequency, and position as a string
	 */
	public String toString() {
		return "\"" + path.toString() + "\"" + ", " + this.frequency + ", "
				+ this.position;
	}

	/**
	 * Takes in the frequency and position and updates the frequency of the
	 * occurrence and checks the position to keep the smallest position
	 * 
	 * @param frequency
	 * @param position
	 */
	public void update(int frequency, int position) {
		this.frequency += frequency;
		if (this.position > position) {
			this.position = position;
		}
	}

	/**
	 * Starts by comparing the frequency, then position, then path and sorts the
	 * searchresults accordingly.
	 */
	@Override
	public int compareTo(SearchResult other) {
		if (Integer.compare(this.frequency, other.frequency) == 0) {
			if (Integer.compare(this.position, other.position) == 0) {
				if (this.path.compareTo(other.path) == 0) {
					return 0;
				} else {
					return this.path.compareTo(other.path);
				}
			} else {
				return Integer.compare(this.position, other.position);
			}
		} else {
			return Integer.compare(other.frequency, this.frequency);

		}
	}

}