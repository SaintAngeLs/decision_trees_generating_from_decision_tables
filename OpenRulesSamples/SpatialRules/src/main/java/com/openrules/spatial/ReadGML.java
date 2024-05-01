package com.openrules.spatial;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.LineNumberReader;
import java.io.Reader;
import java.util.ArrayList;
import java.util.List;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.XMLReaderFactory;

import com.vividsolutions.jts.geom.CoordinateSequence;
import com.vividsolutions.jts.geom.CoordinateSequenceFactory;
import com.vividsolutions.jts.geom.CoordinateSequences;
import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.geom.GeometryFactory;
import com.vividsolutions.jts.geom.LinearRing;
import com.vividsolutions.jts.io.gml2.GMLConstants;
import com.vividsolutions.jts.io.gml2.GMLHandler;

/**
 * Created based on the JTS example KLMReader
 * 
 */
public class ReadGML {
    
    private static Logger Log = LoggerFactory.getLogger(ReadGML.class);
	
	static boolean print = false;
	
	static public void setPrint(boolean b) {
		print = b;
	}

	public static List<Geometry> read(String filename) {
		KMLReader rdr = new KMLReader(filename);
		try {
			List<Geometry> geoms = rdr.read();
			Log.info("Read " + geoms.size() + " geometries");
			return geoms;
		} catch (Exception e) {
			String msg = "Cannot read the GML file: " + filename;
			throw new RuntimeException(msg);
		}
	}

	public static void main(String[] args) throws Exception {
	    
	    String dataPath = System.getProperty("dataPath");
        if (dataPath == null)
            dataPath = System.getProperty("user.dir")+"/data";
        String filename = dataPath + "/counties_us_gml.gml";
//		String filename = ".\\data\\counties_us_gml.gml";
//		String filename = ".\\data\\hospitals.gml";
//        ReadGML.setPrint(true);
		KMLReader rdr = new KMLReader(filename);
		rdr.read();
		System.out.println("OK");
	}
}

class KMLReader {
	private String filename;

	public KMLReader(String filename) {
		this.filename = filename;
	}

	public List<Geometry> read() throws IOException, SAXException {
		XMLReader xr;

		xr = XMLReaderFactory.createXMLReader();
		// xr = new org.apache.xerces.parsers.SAXParser();
		KMLHandler kmlHandler = new KMLHandler();
		xr.setContentHandler(kmlHandler);
		xr.setErrorHandler(kmlHandler);

		Reader r = new BufferedReader(new FileReader(filename));
		LineNumberReader myReader = new LineNumberReader(r);
		xr.parse(new InputSource(myReader));

		return kmlHandler.getGeometries();
		
	}
}

class KMLHandler extends DefaultHandler {
	private List geoms = new ArrayList();

	private GMLHandler currGeomHandler;
	private String lastEltName = null;
	private GeometryFactory fact = new FixingGeometryFactory();

	public KMLHandler() {
		super();
	}

	public List getGeometries() {
		return geoms;
	}

	/**
	 * SAX handler. Handle state and state transitions based on an element
	 * starting.
	 * 
	 * @param uri
	 *            Description of the Parameter
	 * @param name
	 *            Description of the Parameter
	 * @param qName
	 *            Description of the Parameter
	 * @param atts
	 *            Description of the Parameter
	 * @exception SAXException
	 *                Description of the Exception
	 */
	public void startElement(String uri, String name, String qName,
			Attributes atts) throws SAXException {
		if (name.equalsIgnoreCase(GMLConstants.GML_POLYGON) 
			||
			name.equalsIgnoreCase(GMLConstants.GML_POINT)) {
			currGeomHandler = new GMLHandler(fact, null);
		}
		if (currGeomHandler != null)
			currGeomHandler.startElement(uri, name, qName, atts);
		if (currGeomHandler == null) {
			lastEltName = name;
			if (ReadGML.print)
				System.out.println("startElement " + name);
		}
	}

	public void characters(char[] ch, int start, int length)
			throws SAXException {
		if (currGeomHandler != null) {
			currGeomHandler.characters(ch, start, length);
		} else {
			String content = new String(ch, start, length).trim();
			if (content.length() > 0) {
//				if (ReadGML.print)
//					System.out.println(lastEltName + "= " + content);
			}
		}
	}

	public void ignorableWhitespace(char[] ch, int start, int length)
			throws SAXException {
		if (currGeomHandler != null)
			currGeomHandler.ignorableWhitespace(ch, start, length);
	}

	/**
	 * SAX handler - handle state information and transitions based on ending
	 * elements.
	 * 
	 * @param uri
	 *            Description of the Parameter
	 * @param name
	 *            Description of the Parameter
	 * @param qName
	 *            Description of the Parameter
	 * @exception SAXException
	 *                Description of the Exception
	 */
	public void endElement(String uri, String name, String qName)
			throws SAXException {
		if (ReadGML.print)
			System.out.println("endElement " + name);

		if (currGeomHandler != null) {
			currGeomHandler.endElement(uri, name, qName);

			if (currGeomHandler.isGeometryComplete()) {
				Geometry g = currGeomHandler.getGeometry();
				if (ReadGML.print)
					System.out.println(g);
				geoms.add(g);

				// reset to indicate no longer parsing geometry
				currGeomHandler = null;
			}
			else{
				if (ReadGML.print)
					System.out.println("Non-Complete Geometry: uri=" + uri
							+ " name=" + name + " qName=" + qName);
			}
		}

	}
}

/**
 * A GeometryFactory extension which fixes structurally bad coordinate sequences
 * used to create LinearRings.
 * 
 * @author mbdavis
 * 
 */
class FixingGeometryFactory extends GeometryFactory {
	public LinearRing createLinearRing(CoordinateSequence cs) {
		if (cs.getCoordinate(0).equals(cs.getCoordinate(cs.size() - 1)))
			return super.createLinearRing(cs);

		// add a new coordinate to close the ring
		CoordinateSequenceFactory csFact = getCoordinateSequenceFactory();
		CoordinateSequence csNew = csFact.create(cs.size() + 1,
				cs.getDimension());
		CoordinateSequences.copy(cs, 0, csNew, 0, cs.size());
		CoordinateSequences.copyCoord(csNew, 0, csNew, csNew.size() - 1);
		return super.createLinearRing(csNew);
	}

}
