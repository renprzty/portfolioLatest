'use strict';

// Browser adaptation of test.c: same record limits, linear search and bubble sorts.
// The Windows executable and original C file are not executed by this demo.
class StudentDemo {
    constructor(storage) {
        this.storage = storage;
        this.records = [];
        this.notice = '';
        try {
            const saved = JSON.parse(storage.getItem('portfolio-students-v1') || '[]');
            if (!Array.isArray(saved) || saved.length > 100) throw new Error('Invalid records');
            const ids = new Set();
            for (const record of saved) {
                if (!this.validRecord(record) || ids.has(record.nim)) throw new Error('Invalid record');
                ids.add(record.nim);
            }
            this.records = saved;
        } catch {
            this.notice = 'Saved data could not be loaded. This session starts empty.';
        }
        this.restart();
    }
    validRecord(r) {
        return r && ['nim', 'nama', 'jurusan'].every(k => typeof r[k] === 'string' && r[k].trim() && new TextEncoder().encode(r[k]).length <= (k === 'nim' ? 19 : 99)) && Number.isInteger(r.semester) && r.semester >= 1 && r.semester <= 14 && Number.isFinite(r.ipk) && r.ipk >= 0 && r.ipk <= 4;
    }
    save() {
        try { this.storage.setItem('portfolio-students-v1', JSON.stringify(this.records)); this.notice = ''; }
        catch { this.notice = 'Browser storage is unavailable. Changes last for this session only.'; }
    }
    restart() { this.state = 'menu'; this.draft = null; this.editIndex = -1; return this.menu(); }
    menu() {
        this.state = 'menu';
        return '\nSTUDENT DATA MANAGEMENT\n' + '='.repeat(35) + '\n1. Add student\n2. View all students\n3. Search by student ID (NIM)\n4. Edit student\n5. Delete student\n6. Sort records\n7. Student statistics\n0. Exit and save\n\nChoose an option:';
    }
    pause(message) { this.state = 'pause'; return message + '\n\nPress Enter to return to the menu.'; }
    find(nim) { return this.records.findIndex(r => r.nim === nim); }
    record(r) { return `ID: ${r.nim}\nName: ${r.nama}\nMajor: ${r.jurusan}\nSemester: ${r.semester}\nGPA: ${r.ipk.toFixed(2)}`; }
    samples() {
        this.records = [
            { nim: '2026003', nama: 'Charlie', jurusan: 'Information Systems', semester: 3, ipk: 3.2 },
            { nim: '2026001', nama: 'Alex', jurusan: 'Computer Science', semester: 1, ipk: 3.85 },
            { nim: '2026002', nama: 'Jamie', jurusan: 'Informatics', semester: 2, ipk: 2.9 }
        ];
        this.save(); return 'Sample records loaded.\n' + this.restart();
    }
    sort(key, descending = false) {
        for (let i = 0; i < this.records.length - 1; i++) {
            for (let j = 0; j < this.records.length - i - 1; j++) {
                const a = this.records[j][key], b = this.records[j + 1][key];
                if (descending ? a < b : a > b) [this.records[j], this.records[j + 1]] = [this.records[j + 1], this.records[j]];
            }
        }
        this.save();
    }
    statistics() {
        const rs = this.records;
        if (!rs.length) return 'No student records yet.';
        let total = 0, high = rs[0], low = rs[0], honors = 0;
        for (const r of rs) { total += r.ipk; if (r.ipk > high.ipk) high = r; if (r.ipk < low.ipk) low = r; if (r.ipk >= 3.5) honors++; }
        const average = total / rs.length;
        const rating = average >= 3.5 ? 'Excellent' : average >= 3 ? 'Good' : average >= 2.5 ? 'Fair' : 'Needs improvement';
        return `Total students: ${rs.length}\nAverage GPA: ${average.toFixed(2)}\nHighest GPA: ${high.ipk.toFixed(2)} (${high.nama})\nLowest GPA: ${low.ipk.toFixed(2)} (${low.nama})\nGPA >= 3.50: ${honors} (${(honors / rs.length * 100).toFixed(2)}%)\nAssessment: ${rating}`;
    }
    submit(raw) {
        const value = raw.trim();
        if (this.state === 'pause') return this.menu();
        if (this.state === 'exited') return 'Session ended. Use Restart to open the menu.';
        if (this.state === 'menu') {
            if (!/^[0-7]$/.test(value)) return 'Enter a menu number from 0 to 7.';
            switch (value) {
                case '0': this.save(); this.state = 'exited'; return 'Records saved. Thank you! Use Restart to try again.';
                case '1':
                    if (this.records.length >= 100) return this.pause('The maximum of 100 students has been reached.');
                    this.draft = {}; this.editIndex = -1; this.state = 'nim'; return 'ADD STUDENT\nStudent ID (NIM):';
                case '2': return this.pause(this.records.length ? this.records.map((r, i) => `${i + 1}. ${this.record(r)}`).join('\n\n') + `\n\nTotal students: ${this.records.length}` : 'No student records yet.');
                case '7': return this.pause(this.statistics());
                case '6': this.state = 'sort'; return 'SORT RECORDS\n1. Name A–Z\n2. GPA highest–lowest\n3. Student ID ascending\n0. Back\nChoose an option:';
                default:
                    if (!this.records.length) return this.pause('No student records yet.');
                    this.state = value === '3' ? 'search' : value === '4' ? 'edit' : 'delete';
                    return 'Enter student ID (NIM):';
            }
        }
        if (['search', 'edit', 'delete'].includes(this.state)) {
            const index = this.find(value);
            if (index < 0) return this.pause('Student not found.');
            const r = this.records[index];
            if (this.state === 'search') return this.pause('Student found!\n' + this.record(r));
            this.editIndex = index;
            if (this.state === 'delete') { this.state = 'confirm'; return this.record(r) + '\n\nDelete this record? (y/n):'; }
            this.draft = { ...r }; this.state = 'nama'; return 'Current record:\n' + this.record(r) + '\n\nEnter the new name (student ID stays the same):';
        }
        if (this.state === 'confirm') {
            if (!/^[yn]$/i.test(value)) return 'Please enter y or n.';
            if (value.toLowerCase() === 'n') return this.pause('Deletion cancelled.');
            this.records.splice(this.editIndex, 1); this.save(); return this.pause('Student deleted.');
        }
        if (this.state === 'sort') {
            if (value === '0') return this.menu();
            if (!/^[1-3]$/.test(value)) return 'Choose 1, 2, 3, or 0.';
            this.sort(value === '1' ? 'nama' : value === '2' ? 'ipk' : 'nim', value === '2');
            return this.pause('Records sorted and saved. Choose View all students to see the result.');
        }
        if (['nim', 'nama', 'jurusan'].includes(this.state)) {
            const key = this.state, max = key === 'nim' ? 19 : 99;
            if (!value || new TextEncoder().encode(value).length > max || /[|\r\n]/.test(value)) return `Enter a nonempty value up to ${max} bytes, without pipes or line breaks.`;
            if (key === 'nim' && this.find(value) >= 0) return 'This student ID is already registered. Enter another ID:';
            this.draft[key] = value;
            this.state = key === 'nim' ? 'nama' : key === 'nama' ? 'jurusan' : 'semester';
            return { nama: 'Name:', jurusan: 'Major:', semester: 'Semester (1–14):' }[this.state];
        }
        if (this.state === 'semester') {
            if (!/^\d+$/.test(value) || Number(value) < 1 || Number(value) > 14) return 'Semester must be a whole number from 1 to 14.';
            this.draft.semester = Number(value); this.state = 'ipk'; return 'GPA (0.00–4.00):';
        }
        if (this.state === 'ipk') {
            const n = Number(value);
            if (!/^(?:\d+(?:\.\d*)?|\.\d+)$/.test(value) || !Number.isFinite(n) || n < 0 || n > 4) return 'GPA must be a number from 0.00 to 4.00.';
            this.draft.ipk = Number(n.toFixed(2));
            const editing = this.editIndex >= 0;
            if (editing) this.records[this.editIndex] = this.draft; else this.records.push(this.draft);
            this.save(); return this.pause(editing ? 'Student updated.' : 'Student added.');
        }
    }
}

if (typeof document !== 'undefined') {
    let storage;
    try { storage = window.localStorage; } catch { storage = { getItem() { throw new Error(); }, setItem() { throw new Error(); } }; }
    const demo = new StudentDemo(storage);
    const output = document.getElementById('output'), input = document.getElementById('input');
    function write(text, clear = false) {
        if (clear) output.textContent = '';
        output.textContent += text + '\n';
        // Bound terminal history without removing records.
        if (output.textContent.length > 40000) output.textContent = output.textContent.slice(-30000);
        output.scrollTop = output.scrollHeight;
        document.getElementById('storage-status').textContent = demo.notice;
        input.placeholder = demo.state === 'pause' ? 'Press Enter to continue' : demo.state === 'menu' ? 'Choose 0–7' : 'Type your answer';
        input.disabled = demo.state === 'exited';
    }
    write(demo.menu());
    document.getElementById('command').addEventListener('submit', event => {
        event.preventDefault(); const answer = input.value; input.value = '';
        write('> ' + answer + '\n' + demo.submit(answer));
    });
    document.getElementById('restart').addEventListener('click', () => { input.value = ''; write(demo.restart(), true); input.focus(); });
    document.getElementById('samples').addEventListener('click', () => {
        if (demo.records.length && !window.confirm('Replace your demo records with three sample students?')) return;
        input.value = ''; write(demo.samples(), true); input.focus();
    });
}
if (typeof module !== 'undefined') module.exports = { StudentDemo };
