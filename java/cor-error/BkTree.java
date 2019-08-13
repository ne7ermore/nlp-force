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

    public static String correct(String name, Collection<String> expectNames) {
        return correct(name,expectNames,0.75,0.7,2);
    }

    public static String correct(String name, Collection<String> expectNames, double gatherThro, double predThro, int distance) {
        BkTree tree = new BkTree(gatherThro, predThro, distance);
        List<String> pyName = Arrays.asList(Pinyin.toPinyin(name, ",").split(","));
        List<String> newExpectNames = new ArrayList<>();
        List<List<String>> expectPyNames = new ArrayList<>();
        expectNames
                .forEach(expectName -> {
                    newExpectNames.add(expectName);
                    expectPyNames.add(Arrays.asList(Pinyin.toPinyin(expectName, ",").split(",")));
                });
        return tree.divine(name, pyName, newExpectNames, expectPyNames);
    }

    public static String correct(String name, List<String> pyName, List<String> names, List<List<String>> pyNames){
        BkTree tree = new BkTree(0.75, 0.7, 2);
        return tree.divine(name, pyName, names, pyNames);
    }

    public String divine(String name, List<String> pyName, List<String> names, List<List<String>> pyNames) {
        assert names.size() == pyNames.size();
        this.pyDict.clear();

        List<Pair<Integer, String>> samePyNames = new ArrayList<>();
        int idx;

        for (idx = 0; idx < names.size(); idx++) {
            if (name.equals(names.get(idx))) {
                return name;
            }

            if (pyNames.get(idx).equals(pyName)) {
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

        double distance = res.get(0).getKey(), bestTgtThro = 0.0, bestSrcThro = 0.0;
        String pyname = String.join("", pyName), theName = null;

        for (idx = 0; idx < res.size(); idx++) {
            if (res.get(idx).getKey() < distance) {
                break;
            }
            String tName = res.get(idx).getValue();
            if (this.useCustomFilter && this.customFilter.Fillter(tName, name)) continue;
            String tNamePy = String.join("", this.pyDict.get(tName));
            double tgtThro = longestCommonSeq(tNamePy, pyname) * 1.0 / pyname.length();
            double srcThro = longestCommonSeq(tNamePy, pyname) * 1.0 / tNamePy.length();
            if (tgtThro > bestTgtThro) {
                bestTgtThro = tgtThro;
                theName = tName;
                bestSrcThro = srcThro;
            } else if (tgtThro == bestTgtThro && bestSrcThro < srcThro) {
                bestSrcThro = srcThro;
                theName = tName;
            }
        }

        if (bestTgtThro > this.gatherThro && bestSrcThro > this.predThro) {
            return theName;
        }
        return null;
    }

    private List<Pair<Double, String>> search(String word, int radius) {
        List<Pair<Double, String>> res = new ArrayList<>();
        if (this.root.getWord() == null) {
            return res;
        }

        int d, idx;
        double score = 0, tmpScore;
        Queue<Node> queue = new LinkedList<>();
        queue.offer(root);

        while (!queue.isEmpty()) {
            Node n = queue.poll();
            d = editDistance(n.getWord(), word);
            tmpScore = Math.pow(this.powCount, d)*n.getHot();
            if (d <= radius && score <= tmpScore) {
                score = tmpScore;
                res.add(new Pair<>(score, n.getWord()));
            }

            for (idx = Math.max(1, d-radius); idx <= d+radius; idx++) {
                if (n.getChildren().containsKey(idx) && Math.pow(this.powCount, d)*n.getChildren().get(idx).getHot() >= score) {
                    queue.offer(n.getChildren().get(idx));
                }
            }
        }
        return res;
    }

    public void restoreRoot() {
        destroy(this.root);
        this.root = new Node();
    }

    private void destroy(Node node) {
        if (node.getWord() != null) {
            node.setWord(null);
        }

        Iterator iter = node.getChildren().entrySet().iterator();
        while (iter.hasNext()) {
            Map.Entry entry = (Map.Entry) iter.next();
            destroy((Node) entry.getValue());
        }
        node.getChildren().clear();
    }

    public void add(String word, double hot) {
        Node n = this.root;
        if (root.getWord() == null) {
            root.setWord(word);
            root.setHot(hot);
        } else {
            while (true) {
                int d = editDistance(n.getWord(), word);
                if (d == 0) {
                    break;
                }
                if (n.getChildren().containsKey(d)) {
                    n = n.getChildren().get(d);
                } else {
                    n.getChildren().put(d, new Node(word, hot));
                    break;
                }
            }
        }
    }

    private int longestCommonSeq(String py1, String py2) {
        int m = py1.length(), n = py2.length(), prev;
        int[] dp = new int[m+1];
        for (int i = 0; i <= m; i++) {
            dp[i] = 0;
        }
        for (int j = 1; j <= n; j++) {
            for (int i = 1; i <= m; i++) {
                prev = dp[i-1];
                if (py1.charAt(i - 1) == py2.charAt(j - 1)) {
                    dp[i] = prev + 1;
                } else {
                    dp[i] = Math.max(dp[i], dp[i - 1]);
                }
            }
        }
        return dp[m];
    }

    private int editDistance(String word1, String word2) {
        int m = word1.length(), n = word2.length(), tmp, prev;
        int[] dp = new int[m + 1];
        for (int i = 0; i <= m; i++) {
            dp[i] = i;
        }
        for (int i = 1; i <= n; i++) {
            prev = i-1; dp[0] = i;
            for (int j = 1; j <= m; j++) {
                tmp = dp[j];
                if (word1.charAt(j - 1) == word2.charAt(i - 1)) {
                    dp[j] = prev;
                } else {
                    dp[j] = Math.min(Math.min(dp[j], dp[j - 1]), prev) + 1;
                }
                prev = tmp;
            }
        }
        return dp[m];
    }
}
