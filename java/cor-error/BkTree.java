import java.util.*;

public class BkTree {
    private Node root;
    private double powCount, gatherThro, predThro;
    private HashMap<String, List<String>> pyDict;
    private int value;
    private Boolean useCustomFilter;
    private final CustomInterface customFilter;

    public BkTree(double gatherThro, double predThro, int value, CustomInterface customFilter) {
        this.root = new Node();
        this.powCount = 0.1;
        this.value = value;
        this.pyDict = new HashMap<>();
        this.gatherThro = gatherThro;
        this.predThro = predThro;
        this.useCustomFilter = true;
        this.customFilter = customFilter;
    }

    public BkTree(double gatherThro, double predThro, int value) {
        this.root = new Node();
        this.powCount = 0.1;
        this.value = value;
        this.pyDict = new HashMap<>();
        this.gatherThro = gatherThro;
        this.predThro = predThro;
        this.useCustomFilter = false;
        this.customFilter = null;
    }    

    public Node getRoot() {
        return this.root;
    }

    public String divine(String name, List<String> pyName, List<String> names, List<List<String>> pyNames) {
        assert names.size() == pyNames.size();
        this.pyDict.clear();

        List<Pair<Integet, String>> samePyNames = new ArrayList<>();
        int idx;

        for (idx = 0; idx < names.size(); idx++) {
            if (name.equal(names.get(idx))) {
                return name;
            }

            if (pyName.get(idx).equal(pyName)) {
                samePyNames.add(new Pair<>(editDistance(names.get(idx), name), names.get(idx)));
                continue;
            }

            this.pyDict.put(names.get(idx), pyNames.get(idx));
            add(names.get(idx), 1.0);
        }

        if (!samePyNames.isEmpty()) {
            samePyNames.sort(Comparator.comparingInt(Pair::getKey));
            return samePyNames.get(0).getValue();
        }

        List<Pair<Double, String>> res = search(name, this.value);
        restoreRoot();
        if (res.isEmpty()) {
            return null;
        }

        res.sort((o1, o2) -> o1.getKey() - o2.getKey() > 0.0 ? -1:1);

        double 
    }
}