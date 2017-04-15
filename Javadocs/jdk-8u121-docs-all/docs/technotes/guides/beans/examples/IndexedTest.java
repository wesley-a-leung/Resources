import java.beans.IndexedPropertyChangeEvent;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;

import java.awt.Color;

/**
 * Illustrates the basic functionality of IndexedPropertyChangeEvent and the
 * fireIndexed... methods on PropertyChangeSupport.
 * <p>
 * An IndexedPropertyChangeEvent extends the PropertyChangeEvent by adding the
 * index of the property which fired the event.
 */
public class IndexedTest  {

    /**
     * This class represent a souce of IndexedPropertyChangeEvents
     * When an indexed property change, the fireIndexedPropertyChange
     * method should be called.
     */
    public static class IndexedSource {

        private PropertyChangeSupport changeSupport;

        // fields
        private Color color;
        private boolean bool;
        private int ival;

        public IndexedSource() {
            changeSupport = new PropertyChangeSupport(this);
        }

        // Object indexed property
        public void setColor(int index, Color color) {
            Color oldColor = this.color;
            this.color = color;
            changeSupport.fireIndexedPropertyChange("color", index,
                                                    oldColor, color);
        }

        // boolean indexed property
        public void setBoolean(int index, boolean bool) {
            boolean oldBool = this.bool;
            this.bool = bool;
            changeSupport.fireIndexedPropertyChange("boolean", index,
                                                    oldBool, bool);
        }

        // int indexed property
        public void setInteger(int index, int ival) {
            int oldInt = this.ival;
            this.ival = ival;
            changeSupport.fireIndexedPropertyChange("integer", index,
                                                    oldInt, ival);
        }

        /**
         * Listener registration method. Should also implement
         * remove and get listener methods to be complete.
         */
        public void addPropertyChangeListener(PropertyChangeListener l) {
            changeSupport.addPropertyChangeListener(l);
        }
    }

    /**
     * This class represents a listener of PropertyChangeEvents.
     * The new indexed property change notification is compatible with
     * existing property change listeners.
     * <p>
     * If the listener is expecting IndexedPropertyChangeEvents, then
     * the PropertyChangeEvent should be checked.
     */
    public static class IndexedListener implements PropertyChangeListener {

        public void propertyChange(PropertyChangeEvent evt) {
            String propertyName = evt.getPropertyName();

            // Support for indexed properties?
            int index = -1;
            if (evt instanceof IndexedPropertyChangeEvent) {
                index = ((IndexedPropertyChangeEvent)evt).getIndex();
            }
            System.out.println("Property: " + propertyName + " index: " + index +
                               " value: " + evt.getNewValue());
        }
    }

    public static void main(String[] args) throws Exception {
        IndexedSource source = new IndexedSource();
        source.addPropertyChangeListener(new IndexedListener());

        for (int i = 0; i < 5; i++) {
            source.setColor(i, (i % 2 == 0) ? Color.red : Color.blue);
            source.setBoolean(i, (i % 2 == 0) ? true : false);
            source.setInteger(i, i);
        }
    }
}
