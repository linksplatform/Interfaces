// Splits a workflow file into its jobs without a YAML dependency, for the
// workflow policy tests.
export const getJobs = (source) => {
  const jobs = new Map();
  const lines = source.slice(source.indexOf("\njobs:\n") + 1).split("\n");
  let currentName;
  let currentLines = [];

  for (const line of lines.slice(1)) {
    const match = /^  ([A-Za-z_][\w-]*):\s*$/.exec(line);
    if (match) {
      if (currentName) {
        jobs.set(currentName, currentLines.join("\n"));
      }
      currentName = match[1];
      currentLines = [line];
    } else if (currentName) {
      currentLines.push(line);
    }
  }
  if (currentName) {
    jobs.set(currentName, currentLines.join("\n"));
  }
  return jobs;
};
