package com.openrules.spatial;

public class Response {
	
	public String spatialSignificance;
	public int spatialSignificanceScore;
	
	public Response() {
		spatialSignificanceScore = 0;
		spatialSignificance = "?";
	}

	public int getSpatialSignificanceScore() {
		return spatialSignificanceScore;
	}

	public void setSpatialSignificanceScore(int spatialSignificanceScore) {
		this.spatialSignificanceScore = spatialSignificanceScore;
	}

	public String getSpatialSignificance() {
		return spatialSignificance;
	}

	public void setSpatialSignificance(String spatialSignificance) {
		this.spatialSignificance = spatialSignificance;
	}
	
	

}
